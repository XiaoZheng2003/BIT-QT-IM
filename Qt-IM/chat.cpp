#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QString ip,int tid,QString tname,QString tip,QUdpSocket *xchat,qint32 xport,QString lname,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    localIp=ip;
    targetId=tid;
    targetName=tname;
    targetIp=tip;
    this->xchat=xchat;
    this->xport=xport;
    localName=lname;

    connect(&server, SIGNAL(sendFileName(QString)),this,SLOT(getSendFileName(QString)));
    this->setWindowTitle("与"+tname+(tip==""?"":("["+tip+"]"))+"私聊中");
    ui->listWidget->setStyleSheet("QListWidget::item:selected { background-color: transparent; }");

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

void Chat::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
       messageW->setFixedWidth(this->width());
       QSize size = messageW->fontRect(text);
       item->setSizeHint(size);
       messageW->setText(text, time, size, type);
       ui->listWidget->setItemWidget(item, messageW);
}

void Chat::dealMessageTime(QString curMsgTime)
{
    Q_UNUSED(curMsgTime);
     // 不实现时间显示部分
}



void Chat::sendMessage(messageType type,QString serverAddress){
    if(~targetId){
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
                    DBManager::runSql("insert into person_msg (id,msg,time,islocal) values ("+QString::number(targetId)+",'"+message+"','"+time+"',1)");
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

            case RefuseFile:
            {
                out << serverAddress;
                break;
            }
        }
        xchat->writeDatagram(data,data.length(),QHostAddress(targetIp),xport);
    }
    else
    {
        chatRobot();
    }
}

void Chat::refresh()
{
    QSqlQuery query;

    query.exec("select * from person_msg where id="+QString::number(targetId));
    while(query.next()){
        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

        if(query.value(3).toInt()){

            dealMessage(messageW, item, query.value(1).toString(), query.value(2).toString(),  QNChatMessage::User_Me);

        }
        else{
                  dealMessage(messageW, item, query.value(1).toString(), query.value(2).toString(),  QNChatMessage::User_She);
        }
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }
}

void Chat::keyPressEvent(QKeyEvent *ev)
{
    if(ev->modifiers()==Qt::ControlModifier && (ev->key()==Qt::Key_Return||ev->key()==Qt::Key_Enter))
        on_sendMsg_clicked();
}
void Chat::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);


    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
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

File* Chat::getSever() {
    return &server;
}

void Chat::on_history_clicked()
{
    history=new History(targetId,localName,localIp,targetName,targetIp);
    history->setAttribute(Qt::WA_DeleteOnClose);
    history->show();
}

void Chat::on_emote_clicked()
{
    emoji=new Emoji(ui->listWidget->x()+this->x(),
                    ui->listWidget->y()+ui->listWidget->height()+this->y()+frameGeometry().height()-geometry().height());
    emoji->setAttribute(Qt::WA_DeleteOnClose);
    connect(emoji,&Emoji::addEmoji,[=](QString emoji){
       ui->messageTextEdit->insertPlainText(emoji);
    });
    emoji->show();
}

void Chat::chatRobot()
{
    if(ui->messageTextEdit->toPlainText() =="")
    {
        QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
        return ;
    }
    message=getMessage();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    DBManager::runSql("insert into person_msg (id,msg,time,islocal) values ("+QString::number(targetId)+",'"+message+"','"+time+"',1)");
    // 创建网络访问管理器
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    // 发起GET请求
    QNetworkReply *reply = manager->get(QNetworkRequest(
                    QUrl("https://apis.tianapi.com/robot/index?key=0fbd4f91455c68e8fdb30b914d44b79c&question="+message.replace("+","%2B"))));
    // 连接请求完成信号
    connect(reply, &QNetworkReply::finished, [=]() {
        // 读取响应数据
        QByteArray responseData = reply->readAll();
        qDebug()<<responseData;
        // 解析JSON数据
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isNull()) {
            if (jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();
                // 处理JSON对象
                if (jsonObj.contains("result") && jsonObj["result"].isObject()) {
                    // 获取 "result" 对象
                    QJsonObject resultObj = jsonObj["result"].toObject();
                    // 检查是否存在 "reply" 键
                    if (resultObj.contains("reply") && resultObj["reply"].isString()) {
                        // 获取 "reply" 内容
                        QString reply = resultObj["reply"].toString();
                        // 在这里使用 reply 变量，可以打印它或进行其他操作
                        qDebug() << "Reply: " << reply;
                        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                        DBManager::runSql("insert into person_msg (id,msg,time,islocal) values ("+QString::number(targetId)+",'"+reply+"','"+time+"',0)");
                        refresh();
                    }
                }
            }
        }
        // 清理资源
        reply->deleteLater();
    });
}
