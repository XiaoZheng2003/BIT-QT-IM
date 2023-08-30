#include "file.h"
#include "ui_file.h"
#include <QFileDialog>
#include <QMessageBox>
#include "threadmanager.h"

File::File(QString ip,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    this->ip=ip;
    ui->setupUi(this);
    init();
}

File::~File()
{
    delete ui;
}

void File::init()
{
    TotalBytes = 0;
    bytestoWrite = 0;
    bytesWritten = 0;
    ui->serverStatusLabel->setText(tr("请选择文件"));
    ui->progressBar->reset();
    ui->Button_choose->setEnabled(true);
    ui->Button_send->setEnabled(false);
}

void File::sendFile()
{
    ui->Button_send->setEnabled(false);
    ui->serverStatusLabel->setText(tr("开始传送文件:\n %1!").arg(theFileName));
    emit startSend(fileName);
}

void File::updateClientProgress(qint64 numBytes,float timeUsed)
{
    bytesWritten=numBytes;

    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesWritten);

    double speed = bytesWritten / timeUsed;
    ui->serverStatusLabel->setText(tr("已发送 %1MB( %2MB/s)"
                                      "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
                                   .arg(bytesWritten / (1024*1024))
                                   .arg(speed * 1000 /(1024*1024),0,'f',2)
                                   .arg(TotalBytes /(1024*1024))
                                   .arg(timeUsed/1000,0,'f',0)
                                   .arg(TotalBytes/speed/1000 - timeUsed/1000,0,'f',0));
    if(bytesWritten == TotalBytes)
    {
        ui->serverStatusLabel->setText(tr("传送文件: %1成功").arg(theFileName));
        ui->Button_cancle->setText("完成");
        ui->Button_choose->setText("再次选择");
        ui->Button_choose->setEnabled(true);
    }
}

void File::setTotalBytes(qint64 bytes)
{
    TotalBytes=bytes;
    qDebug()<<"total bytes in file"<<TotalBytes;
}

void File::on_Button_choose_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        theFileName = fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
        ui->serverStatusLabel->setText(tr("发送文件:%1").arg(theFileName));
        ui->Button_send->setEnabled(true);
        ui->Button_choose->setEnabled(false);
    }
    else {
        QMessageBox::warning(this,tr("Path"),tr("未选择文件."));
    }
}

void File::refused()
{
    ui->serverStatusLabel->setText(tr("对方拒绝接受文件"));
    ui->Button_choose->setText(tr("重新选择"));
    ui->Button_choose->setEnabled(true);
}

void File::setSocketDescriptor(qint64 socketDescriptor)
{
    m_socketDescriptor=socketDescriptor;
}

void File::closeEvent(QCloseEvent *)
{
    on_Button_cancle_clicked();
}

void File::on_Button_send_clicked()
{
    ui->Button_send->setEnabled(false);
    ui->serverStatusLabel->setText(tr("等待对方的接受......"));
    emit sendFileName(theFileName);
}


void File::on_Button_cancle_clicked()
{
    close();
    emit quit(ip,m_socketDescriptor);
}

//QTcpSocket* File::get_Client_Connection(){
//    return clientConnection;
//}
MyTcpServer *MyTcpServer::m_gMyTcpServer=nullptr;
QMutex MyTcpServer::m_mutex;

MyTcpServer::MyTcpServer(QObject* parent):QTcpServer(parent)
{
}

MyTcpServer::~MyTcpServer()
{
}

void MyTcpServer::init(QObject *parent,qint16 tcpPort)
{
    if(m_gMyTcpServer==nullptr)
    {
        QMutexLocker locker(&m_mutex);
        if(m_gMyTcpServer==nullptr)
            m_gMyTcpServer=new MyTcpServer(parent);
    }
    if(!m_gMyTcpServer->listen(QHostAddress::Any,tcpPort))
    {
        qDebug()<<"MyTcpServer connect failed";
        return ;
    }
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    FileSender *fileSender=new FileSender(socketDescriptor);
    ThreadManager::addObject(QString("fileConnectWith")+QString::number(socketDescriptor),fileSender);
    ThreadManager::startThread(QString("fileConnectWith")+QString::number(socketDescriptor));
    connect(this,&MyTcpServer::startInit,fileSender,&FileSender::init);
    connect(fileSender,&FileSender::startConnect,this,&MyTcpServer::startConnect);
    emit startInit();
}

MyTcpServer *MyTcpServer::getInstance()
{
    return m_gMyTcpServer;
}

void MyTcpServer::addSendTarget(QString targetIp,Chat *chatWindow)
{
    File *fileWindow=new File(targetIp);
    m_gMyTcpServer->m_targetList.insert(targetIp,fileWindow);
    connect(fileWindow,&File::sendFileName,chatWindow,&Chat::getSendFileName);
    fileWindow->init();
    fileWindow->show();
}

void MyTcpServer::startConnect(QString connectIp,qintptr socketDescriptor,FileSender *fileSender)
{
    m_gMyTcpServer->m_targetList.value(connectIp)->setSocketDescriptor(socketDescriptor);
    m_gMyTcpServer->m_fileSenderList.insert(connectIp,fileSender);
    connect(this,&MyTcpServer::prepared,m_gMyTcpServer->m_targetList.value(connectIp),&File::sendFile);
    connect(m_gMyTcpServer->m_targetList.value(connectIp),&File::startSend,m_gMyTcpServer->m_fileSenderList.value(connectIp),&FileSender::startSend);
    connect(m_gMyTcpServer->m_fileSenderList.value(connectIp),&FileSender::sendWrittenBytes,m_gMyTcpServer->m_targetList.value(connectIp),&File::updateClientProgress);
    connect(m_gMyTcpServer->m_fileSenderList.value(connectIp),&FileSender::sendTotalBytes,m_gMyTcpServer->m_targetList.value(connectIp),&File::setTotalBytes);
    connect(m_gMyTcpServer->m_targetList.value(connectIp),&File::quit,m_gMyTcpServer->m_fileSenderList.value(connectIp),&FileSender::quit);
    connect(m_gMyTcpServer->m_targetList.value(connectIp),&File::quit,this,&MyTcpServer::deleteResourse);
    emit prepared();
}

void MyTcpServer::refuse(QString ip)
{
    m_gMyTcpServer->m_targetList.value(ip)->refused();
}

void MyTcpServer::deleteResourse(QString ip,qint64 socketDescriptor)
{
    m_gMyTcpServer->m_targetList.value(ip)->deleteLater();
    m_gMyTcpServer->m_fileSenderList.value(ip)->deleteLater();
    m_gMyTcpServer->m_fileSenderList.remove(ip);
    m_gMyTcpServer->m_targetList.remove(ip);
    ThreadManager::deleteThread(QString("fileConnectWith")+QString::number(socketDescriptor));
}

FileSender::FileSender(qintptr socketDescriptor)
    :QObject(nullptr),
    m_tcpSocket(nullptr),
    m_localFile(nullptr)

{
    m_socketDescriptor=socketDescriptor;
}

void FileSender::startSend(QString fileName)
{
    m_payloadSize = 64 * 1024;
    m_totalBytes = 0;
    m_writtenBytes = 0;
    connect(m_tcpSocket,&QTcpSocket::bytesWritten,this,&FileSender::sendNext);
    m_localFile=new QFile(fileName);
    if(! m_localFile->open(QFile::ReadOnly))
    {
        qDebug()<<tr("警告")<<tr("无法读取文件 %1:\n%2").arg(fileName).arg(m_localFile->errorString());
        return ;
    }
    m_totalBytes=m_localFile->size();
    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_0);
    m_time.start();

    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) <<qint64(0) <<currentFile;

    m_totalBytes +=m_outBlock.size();
    emit sendTotalBytes(m_totalBytes);
    sendOut.device()->seek(0);

    sendOut << m_totalBytes << qint64((m_outBlock.size() - sizeof(qint64)*2));

    m_tcpSocket->write(m_outBlock);
    m_outBlock.resize(0);
}

void FileSender::sendNext(qint64 bytes)
{
    m_writtenBytes+=bytes;
    if(m_writtenBytes<m_totalBytes)
    {
        m_outBlock = m_localFile->read(qMin(m_totalBytes-m_writtenBytes,m_payloadSize));
        m_tcpSocket->write(m_outBlock);
        m_outBlock.resize(0);
    }
    emit sendWrittenBytes(m_writtenBytes,m_time.elapsed());
    if(m_writtenBytes==m_totalBytes)
    {
        quit();
    }
}

void FileSender::init()
{
    m_tcpSocket=new QTcpSocket;
    if(m_tcpSocket->setSocketDescriptor(m_socketDescriptor))
        qDebug()<<"connect success with"+m_tcpSocket->peerAddress().toString();
    else
        qDebug()<<"connect fail";
    QString connectIp=m_tcpSocket->peerAddress().toString();
    connectIp=connectIp.mid(7);
    emit startConnect(connectIp,m_socketDescriptor,this);
}

void FileSender::quit()
{
    if(m_localFile!=nullptr)
    {
        m_localFile->close();
        m_localFile->deleteLater();
    }
    if(m_tcpSocket!=nullptr)
    {
        m_tcpSocket->disconnect();
        m_tcpSocket->close();
        m_tcpSocket->deleteLater();
    }
}
