#include "file.h"
#include "ui_file.h"
#include <QFileDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File),
    localFile(nullptr),
    tcpServer(nullptr),
    clientConnection(nullptr),
    tcpPort(12689)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendFile()));
    init();
}

File::~File()
{
    delete ui;
}

void File::init()
{
    payloadSize = 64 * 1024;
    TotalBytes = 0;
    bytestoWrite = 0;
    bytesWritten = 0;
    ui->serverStatusLabel->setText(tr("请选择文件"));
    ui->progressBar->reset();
    ui->Button_choose->setEnabled(true);
    ui->Button_send->setEnabled(false);
    tcpServer->close();
}

void File::sendFile()
{
    ui->Button_send->setEnabled(false);
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    ui->serverStatusLabel->setText(tr("开始传送文件:\n %1!").arg(theFileName));

    localFile = new QFile(fileName);
    if(! localFile->open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,tr("警告"),tr("无法读取文件 %1:\n%2").arg(fileName).arg(localFile->errorString()));
        return ;
    }
    TotalBytes = localFile->size();

    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_0);
    time.start();

    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) <<qint64(0) <<currentFile;

    TotalBytes +=outBlock.size();
    sendOut.device()->seek(0);

    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytestoWrite = TotalBytes - clientConnection->write(outBlock);
    outBlock.resize(0);
}

void File::updateClientProgress(qint64 numBytes)
{
    qApp->processEvents();
    bytesWritten +=(int) numBytes;
    if(bytestoWrite > 0)
    {
        outBlock = localFile->read(qMin(bytestoWrite,payloadSize));
        bytestoWrite -=(int)clientConnection->write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        localFile->close();
    }

    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesWritten);

    float useTime = time.elapsed();
    double speed = bytesWritten / useTime;
    ui->serverStatusLabel->setText(tr("已发送 %1MB( %2MB/s)"
                                      "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
                                   .arg(bytesWritten / (1024*1024))
                                   .arg(speed * 1000 /(1024*1024),0,'f',2)
                                   .arg(TotalBytes /(1024*1024))
                                   .arg(useTime/1000,0,'f',0)
                                   .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0));
    if(bytesWritten == TotalBytes)
    {
        localFile->close();
        tcpServer->close();
        ui->serverStatusLabel->setText(tr("传送文件: %1成功").arg(theFileName));
    }
}
/*
void File::on_Button_choose_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("选择一个文件"),".",tr("All Files(*.*)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,tr("打开文件"),
            tr("打开文件失败:\n%1").arg(path));
            return;
        }
        //QTextStream in(&file);
        fname=path;
        if(!fname.isEmpty())
        {
            QMessageBox::information(this,tr("信息"),tr("选择文件成功."));
            QStringList tmp=fname.split('/');
            ui->file->setText(tmp[tmp.length()-1]);
        }
        //ui->textEdit->setText(in.readAll());
        file.close();

    }else {
        QMessageBox::warning(this,tr("Path"),tr("未选择文件."));
    }
}
*/

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
    tcpServer->close();
    ui->serverStatusLabel->setText(tr("对方拒绝接受文件"));
    ui->Button_choose->setText(tr("重新选择"));
    ui->Button_choose->setEnabled(true);
}

void File::closeEvent(QCloseEvent *)
{
    on_Button_cancle_clicked();
}

void File::on_Button_send_clicked()
{
    if(!tcpServer->listen(QHostAddress::Any,tcpPort))
    {
        QMessageBox::warning(this,tr("Fail"),tr("连接失败."));
        close();
        return ;
    }
    ui->Button_send->setEnabled(false);
    ui->serverStatusLabel->setText(tr("等待对方的接受......"));
    emit sendFileName(theFileName);
}


void File::on_Button_cancle_clicked()
{
    if(tcpServer->isListening())
    {
        tcpServer->close();
        if(localFile!=nullptr){
            if(localFile->isOpen())
            {
                localFile->close();
            }
        }
        if(clientConnection!=nullptr){
            clientConnection->abort();
        }

    }
    close();
}



