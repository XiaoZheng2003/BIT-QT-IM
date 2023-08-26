#include "chat.h"
#include "ui_chat.h"

Chat::Chat(bool isGroup,int tid,QString tname,QString tip,QUdpSocket *xchat,qint32 xport,QString lname,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    localIp=NetworkTool::GetLocalIP();
    connect(&server, SIGNAL(sendFileName(QString)),this,SLOT(getSendFileName(QString)));
    this->setWindowTitle("与"+tname+"["+tip+"]私聊中");

    localIp=NetworkTool::GetLocalIP();
    this->isGroup=isGroup;
    targetId=tid;
    targetName=tname;
    targetIp=tip;
    this->xchat=xchat;
    this->xport=xport;
    localName=lname;
}

Chat::~Chat()
{
    delete ui;
}

QString Chat::getMessage()
{
    QString msg = ui->messageTextEdit->toPlainText();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

void Chat::sendMessage(messageType type,QString serverAddress){
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << type << localIp;
    switch (type) {
        case PersonMessage:
        {
            if(ui->messageTextEdit->toPlainText() =="")
            {
                QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
                return ;
            }
            else
            {
                //ui->label->setText(tr("与%1私聊中 对方的IP：%2").arg(targetName).arg(targetIp));
                message = getMessage();
                out << message;
                QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                DBManager::runSql("insert into msg (type,id,msg,time,islocal) values (0,"+QString::number(targetId)+",'"+message+"','"+time+"',1)");
                qDebug()<<"send msg:"+data;
            }
            break;
        }

        case SendFileName:
        {
            QString clientAddresss = targetIp;
            out << clientAddresss << fileName;
            break;
        }
    }
    xchat->writeDatagram(data,data.length(),QHostAddress(targetIp),xport);
}

void Chat::refresh()
{
    QSqlQuery query;
    QTextBrowser *tb=ui->messageTextBrowser;
    tb->clear();
    query.exec("select * from msg where id="+QString::number(targetId));
    while(query.next()){
        tb->setCurrentFont(QFont("黑体",8));
        if(query.value(4).toInt()){
            tb->setTextColor(Qt::blue);
            tb->append(localName+"["+localIp+"]"+query.value(3).toString());
        }
        else{
            tb->setTextColor(Qt::green);
            tb->append(targetName+"["+targetIp+"]"+query.value(3).toString());
        }


        tb->append(query.value(2).toString());
    }
}

void Chat::on_sendMsg_clicked()
{
    sendMessage(PersonMessage);
    refresh();
}

// 传输文件按钮槽函数
void Chat::on_file_clicked()
{
    server.show();
    server.init();
}

// 接收传输文件信息槽函数
void Chat::getSendFileName(QString name)
{
    fileName = name;
    sendMessage(SendFileName);
}


void Chat::on_closeBtn_clicked()
{
    this->close();
}
