#include "group.h"
#include "ui_group.h"

Group::Group(HeartbeatBroadcaster * reci, QString ip, QUdpSocket *xchat, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Group),
    xport(23333)
{
    ui->setupUi(this);
    receiver = reci;
    refresh();
    localIp = ip;
    groupId = -1;
    this->xchat=xchat;
    // 将信号 personListChanged() 与槽函数 refresh() 连接起来
    connect(receiver, &HeartbeatBroadcaster::personListChanged, this, &Group::refresh);

    initMenu();
}

Group::~Group()
{
    delete ui;
}

void Group::keyPressEvent(QKeyEvent *ev)
{
    if(ev->modifiers()==Qt::ControlModifier && (ev->key()==Qt::Key_Return||ev->key()==Qt::Key_Enter))
        on_sendMsg_clicked();
}

void Group::initMenu()
{
    QMenu *popMenu=new QMenu(this);
    QAction *addNode=new QAction("添加好友",this);
    ui->list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->list,&QTreeWidget::customContextMenuRequested,[=](QPoint pos){
        QTreeWidgetItem *curItem=ui->list->itemAt(pos);
        if(curItem==nullptr) return ;
        popMenu->clear();
        popMenu->addAction(addNode);
        QAction* select=popMenu->exec(QCursor::pos());
        if(select==addNode){
            //添加好友
            QString ip=curItem->text(1);
            if(NetworkTool::isIpExist(ip)){
                QMessageBox::critical(this,tr("错误"),tr("同IP好友禁止重复添加！"));
                return;
            }
            if(QMessageBox::question(this,"提示",QString("确认添加好友[%1]？").arg(ip),
                                     QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
                DBManager::runSql(
                            QString("insert into person (nickname,ip,avatar) values ('%1','%2',%3)")
                            .arg(ip).arg(ip).arg(QRandomGenerator::global()->bounded(6)));
                emit addPerson();
            }
        }
    });
}

void Group::sendMessage(messageType type)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString message;

    out << type << localIp;
    switch (type) {
    case GroupMessage:
    {
        if(ui->messageTextEdit->toPlainText() =="")
        {
            QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
            return ;
        }
        else
        {
            message = getMessage();
            out << groupId << message;
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            //DBManager::runSql("insert into group_msg (id,msg,time,islocal,ip) values ("+QString::number(targetId)+",'"+message+"','"+time+"',1)");
            DBManager::runSql(QString("insert into group_msg (id,msg,time,islocal,ip)"
                                      " values (%1,'%2','%3',1,'%4')").arg(groupId).arg(message).arg(time).arg(localIp));
            qDebug()<<"send msg:"+data;
        }
        break;
    }
    }
    xchat->writeDatagram(data,data.length(),QHostAddress::Broadcast,xport);
}

void Group::refresh()
{
    QTreeWidget *List=ui->list;
    List->clear();
    List->setColumnWidth(0,150);
    List->setIconSize(QSize(25,25));
    for (int i = 0; i < receiver->getReceivedIPs().size(); ++i) {
        //qDebug() << receiver->getReceivedIPs().at(i);
        QTreeWidgetItem *person=new QTreeWidgetItem(List);
        person->setIcon(0,QIcon(QString(":/res/avatar%1.png").arg(QRandomGenerator::global()->bounded(6))));
        person->setText(1,receiver->getReceivedIPs().at(i));
    }
}

QString Group::getMessage()
{
    QString msg = ui->messageTextEdit->toPlainText();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

void Group::reloadMessage()
{
    QSqlQuery query;
    QTextBrowser *tb=ui->messageTextBrowser;
    tb->clear();
    query.exec("select * from group_msg where id="+QString::number(groupId));
    while(query.next()){
        tb->setCurrentFont(QFont("黑体",8));
        if(query.value(3).toInt())
            tb->setTextColor(Qt::blue);
        else
            tb->setTextColor(Qt::green);
        tb->append(query.value(4).toString()+" "+query.value(2).toString());
        tb->append(query.value(1).toString());
    }
}

void Group::on_closeBtn_clicked()
{
    this->close();
}

void Group::on_sendMsg_clicked()
{
    sendMessage(GroupMessage);
    reloadMessage();
}

void Group::on_emote_clicked()
{
    emoji=new Emoji(ui->messageTextBrowser->x()+this->x(),
                    ui->messageTextBrowser->y()+ui->messageTextBrowser->height()+this->y()+frameGeometry().height()-geometry().height());
    emoji->setAttribute(Qt::WA_DeleteOnClose);
    connect(emoji,&Emoji::addEmoji,[=](QString emoji){
       ui->messageTextEdit->insertPlainText(emoji);
    });
    emoji->show();
}

void Group::on_history_clicked()
{
    history=new History(groupId);
    history->setAttribute(Qt::WA_DeleteOnClose);
    history->show();
}
