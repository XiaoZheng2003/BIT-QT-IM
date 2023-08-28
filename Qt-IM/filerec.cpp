#include "filerec.h"
#include "ui_filerec.h"
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>
#include <QFileDialog>


Filerec::Filerec(QWidget *parent) :
    QWidget(parent),
    tcpClient(nullptr),
    localFile(nullptr),
    TotalBytes(0),
    bytesReceived(0),
    fileNameSize(0),
    tcpPort(12689),
    ui(new Ui::Filerec)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

Filerec::~Filerec()
{
    delete ui;
}

void Filerec::setFileName(QString fileName)
{
    localFile = new QFile(fileName);
}

void Filerec::setHostAddress(QHostAddress address)
{
    hostAddress = address;
    newConnect();
}

void Filerec::newConnect()
{
    blockSize = 0;
    tcpClient->abort();
    tcpClient->connectToHost(hostAddress,tcpPort);
    time.start();
}

void Filerec::readMessage()
{
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_4_0);
    float useTime = time.elapsed();
    if(bytesReceived <= sizeof(qint64)*2)
    {
        if((tcpClient->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize ==0))
        {
            in >> TotalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tcpClient->bytesAvailable() >= fileNameSize) && (fileNameSize!=0))
        {
            in >> fileName;
            bytesReceived += fileNameSize;
            if(! localFile->open(QFile::WriteOnly))
            {
                QMessageBox::warning(this,tr("警告"),tr("无法读取文件 %1:\n%2.").arg(fileName).arg(localFile->errorString()));
                return ;
            }
            else return ;
        }
    }
    if(bytesReceived < TotalBytes)
    {
        bytesReceived += tcpClient->bytesAvailable();
        inBlock = tcpClient->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesReceived);

    double speed = bytesReceived / useTime;
    ui->statusLabel->setText(tr("已接收 %1MB( %2MB/s)"
                                         "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
                                      .arg(bytesReceived / (1024*1024))
                                      .arg(speed *1000/(1024*1024),0,'f',2)
                                      .arg(TotalBytes / (1024*1024))
                                      .arg(useTime/1000,0,'f',0)
                                      .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0 ));
    if(bytesReceived == TotalBytes)
    {
       localFile ->close();
       tcpClient->close();
       ui->statusLabel->setText(tr("接收文件: %1完毕").arg(fileName));
    }
}

void Filerec::displayError(QAbstractSocket::SocketError sockError)
{
    qDebug() << tcpClient->errorString();
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
    tcpClient->abort();
    if(localFile->isOpen())
    {
       localFile->close();
    }
    close();
}


void Filerec::on_finishButton_clicked()
{
    on_cancelButton_clicked();
}

