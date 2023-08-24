#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Linpop");

    ui->ipAddress->setText("IP地址："+NetworkTool::GetLocalIP());
    refresh();

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
    personList->setIconSize(QSize(25,25));
    QSqlQuery query;
    query.exec("select * from person");
    while(query.next()){
        QTreeWidgetItem *person=new QTreeWidgetItem(personList);
        person->setText(0,query.value(1).toString());
        person->setIcon(0,QIcon(":/res/avatar.jpg"));
        person->setText(1,query.value(2).toString());
    }

    //群聊页面
    QTreeWidget *groupList=ui->groupList;
    groupList->clear();
    groupList->setColumnWidth(0,150);
    groupList->setIconSize(QSize(25,25));
    query.exec("select * from groups");
    while(query.next()){
        QTreeWidgetItem *group=new QTreeWidgetItem(groupList);
        group->setText(0,query.value(1).toString());
        group->setIcon(0,QIcon(":/res/group.png"));
        group->setText(1,query.value(3).toString());
    }
}
