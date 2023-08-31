#include "filerec.h"
#include "ui_filerec.h"
#include "threadmanager.h"
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>
#include <QFileDialog>
#include <QThread>


Filerec::Filerec(QWidget *parent) :
    QWidget(parent),
    tcpPort(12689),
    totalBytes(0),
    bytesReceived(0),
    ui(new Ui::Filerec)
{
    ui->setupUi(this);
}

Filerec::~Filerec()
{
    delete ui;
}

void Filerec::setFileName(QString fileName)
{
    this->fileName=fileName;
}

void Filerec::setHostAddress(QHostAddress address)
{
    hostAddress = address;
    newConnect();
}

void Filerec::newConnect()
{
    fileReceiver =new FileReceiver(hostAddress,tcpPort);
    ThreadManager::addObject(QString("fileConnectWith")+hostAddress.toString(),fileReceiver);
    ThreadManager::startThread(QString("fileConnectWith")+hostAddress.toString());
    connect(fileReceiver,&FileReceiver::sendReceivedBytes,this,&Filerec::getReceivedBytes);
    connect(fileReceiver,&FileReceiver::sendTotalBytes,this,&Filerec::setTotalBytes);
    connect(this,&Filerec::startConnect,fileReceiver,&FileReceiver::startConnect);
    connect(fileReceiver,&FileReceiver::endConnect,this,&Filerec::endConnect);
    emit startConnect(fileName);
}

void Filerec::endConnect()
{
    ThreadManager::deleteThread(QString("fileConnectWith")+hostAddress.toString());
    fileReceiver->deleteLater();
    ui->statusLabel->setText(tr("接收文件: %1完毕").arg(fileName));
}


void Filerec::setTotalBytes(qint64 totalBytes)
{
    this->totalBytes=totalBytes;
}
void Filerec::getReceivedBytes(qint64 bytes,float timeUsed)
{
    bytesReceived=bytes;
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesReceived);

    double speed = bytesReceived / timeUsed;
    ui->statusLabel->setText(tr("已接收 %1MB( %2MB/s)"
                                         "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
                                      .arg(bytesReceived / (1024*1024))
                                      .arg(speed *1000/(1024*1024),0,'f',2)
                                      .arg(totalBytes / (1024*1024))
                                      .arg(timeUsed/1000,0,'f',0)
                                      .arg(totalBytes/speed/1000 - timeUsed/1000,0,'f',0 ));
}

void Filerec::closeEvent(QCloseEvent *)
{
    on_finishButton_clicked();
}

/*void Filerec::on_button_normal_clicked()
{

    QString filename = QFileDialog::getExistingDirectory();
    QDir *dir=new QDir(filename);
    QStringList filter;
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    for(int i = 0;i<fileInfo->count(); i++)
    {
        qDebug()<<fileInfo->at(i).filePath();
        qDebug()<<fileInfo->at(i).fileName();
    }
    QString  sub="temp";
    //dir.mkdir(sub);
    ui->pathname->setText(filename+"/"+sub+"/");
}*/


void Filerec::on_cancelButton_clicked()
{
    ThreadManager::deleteThread(QString("fileConnectWith")+hostAddress.toString());
    fileReceiver->deleteLater();
    close();
}


void Filerec::on_finishButton_clicked()
{
    on_cancelButton_clicked();
}

FileReceiver::FileReceiver(QHostAddress address,qint16 port):
    QObject(nullptr),
    fileNameSize(0),
    receivedBytes(0)
{
    hostAddress=address;
    tcpPort=port;
}

void FileReceiver::startConnect(QString fileName)
{
    this->fileName=fileName;
    localFile=new QFile(fileName);
    tcpClient=new QTcpSocket(this);
    tcpClient->abort();
    tcpClient->connectToHost(hostAddress,tcpPort);
    connect(tcpClient,&QTcpSocket::readyRead,this,&FileReceiver::readMessage);
    time.start();
}


void FileReceiver::readMessage()
{
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_4_0);
    float timeUsed = time.elapsed();
    if(receivedBytes <= sizeof(qint64)*2)
    {
        if((tcpClient->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize ==0))
        {
            in >> totalBytes >> fileNameSize;
            emit sendTotalBytes(totalBytes);
            receivedBytes += sizeof(qint64)*2;
        }
        if((tcpClient->bytesAvailable() >= fileNameSize) && (fileNameSize!=0))
        {
            in >> fileName;
            receivedBytes += fileNameSize;
            if(! localFile->open(QFile::WriteOnly))
            {
                //QMessageBox::warning(this,tr("警告"),tr("无法读取文件 %1:\n%2.").arg(fileName).arg(localFile->errorString()));
                return ;
            }
            else return ;
        }
    }
    if(receivedBytes < totalBytes)
    {
        receivedBytes += tcpClient->bytesAvailable();
        inBlock = tcpClient->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    emit sendReceivedBytes(receivedBytes,timeUsed);
    if(receivedBytes == totalBytes)
    {
        localFile ->close();
        localFile->deleteLater();
        tcpClient->close();
        tcpClient->deleteLater();
        emit endConnect();
    }
}

void FileReceiver::displayError(QAbstractSocket::SocketError socktError)
{
    qDebug() << tcpClient->errorString();
}
