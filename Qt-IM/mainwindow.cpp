#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    xport(23333)
{
    m_editWindow=new Edit(this);

    ui->setupUi(this);
    this->setWindowTitle("Linpop");

    localIp=NetworkTool::GetLocalIP();
    ui->ipAddress->setText("IP地址："+localIp);
    refresh();
    initMenu();

    connect(this,&MainWindow::receiveMsg,&chat,&Chat::refresh);
    connect(ui->personList,&QTreeWidget::itemDoubleClicked,[=](QTreeWidgetItem *item){
        //打开个人聊天界面
        chat.init(0,item->text(2).toInt(),item->text(0),item->text(1),xchat,xport);
        chat.show();
        emit receiveMsg();
        qDebug()<<item->text(0)<<item->text(1); //昵称 IP
    });

    connect(ui->groupList,&QTreeWidget::itemDoubleClicked,[=](QTreeWidgetItem *item){
        //打开群聊界面
        qDebug()<<item->text(0)<<item->text(2); //群名 IP
    });

    xchat = new QUdpSocket(this);
    xchat->bind(QHostAddress(localIp),xport);

    connect(xchat,&QUdpSocket::readyRead,this,&MainWindow::processPendinDatagrams);
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
                emit receiveMsg();
                qDebug()<<"receive msg.";
                in >> ip >> msgStr;
                QSqlQuery query;
                query.exec("select id from person where ip='"+ip+"'");
                QString id="";
                while(query.next()){
                    id=query.value(0).toString();
                }
                if(id==""){
                    DBManager::runSql("insert into person (nickname,ip) values ('好友','"+ip+"')");
                    query.exec("select id from person where ip='"+ip+"'");
                    while(query.next()){
                        id=query.value(0).toString();
                    }
                }
                DBManager::runSql("insert into msg (type,id,msg,time,islocal) values (0,"+id+",'"+msgStr+"','"+time+"',0)");
                refresh();
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
        }
    }
}

void MainWindow::hasPendinFile(QString serverAddress, QString clientAddress, QString fileName)
{
    // 每个客户端都检查，可优化
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
                Filerec *client = new Filerec(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();
            }
        }
        else if(btn == QMessageBox::No)
        {
            //sendMessage(RefuseFile,serverAddress);
        }
    }
}
void MainWindow::getUsername(QString un)
{
    ui->nickname->setText(un);
}

void MainWindow::refresh()
{
    //单聊页面
    QTreeWidget *personList=ui->personList;
    personList->clear();
    personList->setColumnWidth(0,150);
    personList->setColumnHidden(2,true);
    personList->setIconSize(QSize(25,25));
    QSqlQuery query;
    query.exec("select * from person");
    while(query.next()){
        QTreeWidgetItem *person=new QTreeWidgetItem(personList);
        person->setText(0,query.value(1).toString());
        person->setIcon(0,QIcon(":/res/avatar.jpg"));
        person->setText(1,query.value(2).toString());
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
        popMenu->clear();
        popMenu->addAction(editNode);
        popMenu->addAction(deleteNode);
        QAction* select=popMenu->exec(QCursor::pos());
        if(select==editNode){
            //编辑
            m_editWindow->init(curItem->text(2).toInt(),false);
            m_editWindow->exec();
            qDebug()<<"编辑序号"<<curItem->text(2);
            //TODO: 弹出编辑框
            refresh();
        }
        else if(select==deleteNode){
            //删除
            if(QMessageBox::question(this,"提示","确认删除该好友吗？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
                DBManager::runSql("delete from person where id="+curItem->text(2));
                refresh();
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
