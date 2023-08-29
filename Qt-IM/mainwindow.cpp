#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QString ip, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    xport(23333)
{
    m_editWindow=new Edit(this);

    ui->setupUi(this);
    this->setWindowTitle("Linpop");

    localName=username;
    ui->nickname->setText(username);
    localIp=ip;
    ui->ipAddress->setText("IP地址："+localIp);
    refresh();
    initMenu();

    //ui->personList->setHeaderHidden(true);
    //ui->groupList->setHeaderHidden(true);
     ui->personList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
     ui->groupList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->personList,&QTreeWidget::itemDoubleClicked,[=](QTreeWidgetItem *item){
        if(item->isDisabled()){
            QMessageBox::critical(this,"错误","该用户为离线状态！");
            return;
        }
        //打开个人聊天界面
        chat=new Chat(ip,item->text(2).toInt(),item->text(0),item->text(1),xchat,xport,localName);
        connect(this,&MainWindow::receivePersonMsg,chat,&Chat::refresh);
        chat->setAttribute(Qt::WA_DeleteOnClose);
        chat->show();
        emit receivePersonMsg();
    });

    connect(ui->groupList,&QTreeWidget::itemDoubleClicked,[=](QTreeWidgetItem *item){
        //打开群聊界面
        group=new Group(broadcaster,localIp,xchat);
        connect(group,&Group::addPerson,[=](){
            refresh();updateOnline();
        });
        connect(this,&MainWindow::receiveGroupMsg,group,&Group::reloadMessage);
        group->setAttribute(Qt::WA_DeleteOnClose);
        group->show();
        emit receiveGroupMsg();
    });

    receiver = new HeartbeatReceiver(localIp);
    broadcaster = new HeartbeatBroadcaster;

    xchat = new QUdpSocket(this);
    xchat->bind(QHostAddress(localIp),xport);

    connect(xchat,&QUdpSocket::readyRead,this,&MainWindow::processPendinDatagrams);
    connect(broadcaster,&HeartbeatBroadcaster::personListChanged,this,&MainWindow::updateOnline);

//    Group *gr = new Group(broadcaster);
//    gr->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_clicked()
{
    //创建添加好友/群窗口
    Add *a=new Add(this);
    a->setAttribute(Qt::WA_DeleteOnClose);
    a->exec();
    refresh();
    updateOnline();
}

void MainWindow::processPendinDatagrams()
{
    while (xchat->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(xchat->pendingDatagramSize());
        xchat->readDatagram(datagram.data(),datagram.size());

        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in >> msgType;
        QString ip,msgStr;

        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch (msgType) {
            case PersonMessage:
            {
                qDebug()<<"receive msg.";
                in >> ip >> msgStr;
                QSqlQuery query;
                query.exec("select id from person where ip='"+ip+"'");
                QString id="";
                while(query.next()){
                    id=query.value(0).toString();
                }
                if(id==""){
                    DBManager::runSql(QString("insert into person (nickname,ip) values ('%1','%2')").arg(ip).arg(ip));
                    query.exec("select id from person where ip='"+ip+"'");
                    while(query.next()){
                        id=query.value(0).toString();
                    }
                }
                DBManager::runSql("insert into person_msg (id,msg,time,islocal) values ("+id+",'"+msgStr+"','"+time+"',0)");
                refresh();
                updateOnline();
                emit receivePersonMsg();
            }
            case GroupMessage:
            {
                QString id;
                qDebug()<<"receive msg.";
                in >> ip >> id >> msgStr;
                if(ip==localIp) break;
                DBManager::runSql(QString("insert into group_msg (id,msg,time,islocal,ip)"
                                          " values (%1,'%2','%3',0,'%4')").arg(-1).arg(msgStr).arg(time).arg(ip));
                refresh();
                updateOnline();
                emit receiveGroupMsg();
            }
            case SendFileName:
            {
                QString localIp, clientAddress, fileName;

                in >> localIp >> clientAddress >> fileName ;
                qDebug() << "localIp: " << localIp;
                qDebug() << "clientAddress: " << clientAddress;
                qDebug() << "fileName: " << fileName;
                hasPendinFile(localIp,clientAddress,fileName);
                /*this->show();
                setWindowTitle("私聊");*/
                break;
            }
            case RefuseFile:
            {
                QString clinetIp, serverAddress;
                in >> clinetIp >> serverAddress;
                QString ipAddress = localIp;
                if(ipAddress == serverAddress)
                {
                    chat->getSever()->refused();
                }
//                else if(ipAddress == "255.255.255.255"){
//                    if(chat->getSever()->get_Client_Connection()->localAddress().toString()==clinetIp) // 找到对应关闭窗口客户端的服务端
//                    chat->getSever()->refused();
//                }

                break;
            }
        }
    }
}

void MainWindow::hasPendinFile(QString serverAddress, QString clientAddress, QString fileName)
{
    if(clientAddress == localIp)
    {
        int btn = QMessageBox::information(this,tr("接收文件"),
                                           tr("来自 %1 的文件:%2","是否接受")
                                           .arg(serverAddress).arg(fileName),
                                           QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty())
            {
                Filerec *client = new Filerec();
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();
            }
            else{
                if(chat == nullptr){
                    Chat* refusechat = new Chat(localIp,-999,"", serverAddress, xchat, xport, localName); // 发送拒绝消息的临时chat
                    refusechat->sendMessage(RefuseFile,serverAddress);
                    delete refusechat;
                }
                else{
                    chat->sendMessage(RefuseFile,serverAddress);
                }
            }
        }
        else if(btn == QMessageBox::No)
        {
            if(chat == nullptr){
                Chat* refusechat = new Chat(localIp,-999,"", serverAddress, xchat, xport, localName); // 发送拒绝消息的临时chat
                refusechat->sendMessage(RefuseFile,serverAddress);
                delete refusechat;
            }
            else{
                chat->sendMessage(RefuseFile,serverAddress);
            }
        }
    }
}

void MainWindow::refresh()
{
    //单聊页面
    //receiver->onlineIPs
    QTreeWidget *personList=ui->personList;
    personList->clear();
    personList->setColumnWidth(0,150);
    personList->setColumnHidden(2,true);
    personList->setIconSize(QSize(25,25));
    QSqlQuery query;
    query.exec("select * from person");
    //QList<QString> onlineIPs=broadcaster->getReceivedIPs();
    while(query.next()){
        QTreeWidgetItem *person=new QTreeWidgetItem(personList);
        QString ip=query.value(2).toString();
        person->setText(0,query.value(1).toString());
        person->setText(1,ip);
        QImage image(QString(":/res/avatar%1.png").arg(query.value(3).toInt()));
        //if(ip!="" && onlineIPs.contains(ip))
            person->setDisabled(true);
        person->setIcon(0, QIcon(QPixmap::fromImage(image)));
        person->setText(2,query.value(0).toString());
    }

    //群聊页面
    QTreeWidget *groupList=ui->groupList;
    groupList->clear();
    groupList->setColumnWidth(0,150);
    groupList->setColumnHidden(2,true);
    groupList->setColumnHidden(3,true);
    groupList->setIconSize(QSize(25,25));
    query.exec("select * from groups");
    while(query.next()){
        QTreeWidgetItem *group=new QTreeWidgetItem(groupList);
        group->setText(0,query.value(1).toString());
        group->setIcon(0,QIcon(":/res/group.png"));
        group->setText(1,query.value(3).toString());
        group->setText(2,query.value(2).toString());
        group->setText(3,query.value(0).toString());
    }
}

void MainWindow::initMenu()
{
    QMenu *popMenu=new QMenu(this);
    QAction *editNode=new QAction("编辑",this);
    QAction *deleteNode=new QAction("删除",this);

    //单聊菜单
    ui->personList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->personList,&QTreeWidget::customContextMenuRequested,[=](QPoint pos){
        QTreeWidgetItem *curItem=ui->personList->itemAt(pos);
        if(curItem==nullptr) return ;
        if(curItem->text(2)=="-1"){
            editNode->setDisabled(true);
            deleteNode->setDisabled(true);
        }
        else{
            editNode->setEnabled(true);
            deleteNode->setEnabled(true);
        }
        popMenu->clear();
        popMenu->addAction(editNode);
        popMenu->addAction(deleteNode);
        QAction* select=popMenu->exec(QCursor::pos());
        if(select==editNode){
            //编辑
            m_editWindow->init(curItem->text(2).toInt(),false);
            m_editWindow->exec();
            qDebug()<<"编辑序号"<<curItem->text(2);
            refresh();
            updateOnline();
        }
        else if(select==deleteNode){
            //删除
            if(QMessageBox::question(this,"提示","确认删除该好友吗？（同时会删除该好友的聊天记录）",
                                     QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
                DBManager::runSql("delete from person where id="+curItem->text(2));
                refresh();
                updateOnline();
            }
        }
    });

    //群聊菜单
    ui->groupList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->groupList,&QTreeWidget::customContextMenuRequested,[=](QPoint pos){
        QTreeWidgetItem *curItem=ui->groupList->itemAt(pos);
        if(curItem==nullptr) return ;
        popMenu->clear();
        popMenu->addAction(editNode);
        popMenu->addAction(deleteNode);
        QAction* select=popMenu->exec(QCursor::pos());
        if(select==editNode){
            //编辑
            m_editWindow->init(curItem->text(3).toInt(),true);
            m_editWindow->exec();
            qDebug()<<"编辑序号"<<curItem->text(3);
            //TODO: 弹出编辑框
            refresh();
        }
        else if(select==deleteNode){
            //删除
            if(QMessageBox::question(this,"提示","确认删除该群聊吗？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
                DBManager::runSql("delete from groups where id="+curItem->text(3));
                refresh();
            }
        }
    });
}

void MainWindow::updateOnline()
{
    //单聊页面在线状态显示
    QTreeWidget *personList=ui->personList;
    QTreeWidgetItemIterator it(personList);
    QList<QString> onlineIps=broadcaster->getReceivedIPs();
    while(*it){
        if(onlineIps.contains((*it)->text(1))||(*it)->text(1)=="")
            (*it)->setDisabled(false);
        else
            (*it)->setDisabled(true);
        ++it;
    }
}

//void MainWindow::closeEvent(QCloseEvent *) // 关闭窗口，向所有玩家发送拒绝信号
//{
//    Chat* refusechat = new Chat(localIp,-999,"", "255.255.255.255", xchat, xport, localName); // 发送拒绝消息的临时chat
//    refusechat->sendMessage(RefuseFile,"255.255.255.255"); // 广播
//    delete refusechat;
//}

void MainWindow::on_avatar_clicked()
{
    a.init(avatarId);
    a.show();
    connect(&a,&Avatar::changeAvatarId,[=](int aid){
        avatarId=aid;
        ui->avatar->setIcon(QIcon(QPixmap(QString(":/res/avatar%1.png").arg(aid))));
    });
}
