#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Linpop");

    ui->ipAddress->setText("IP地址："+NetworkTool::GetLocalIP());

    //单聊页面
    QTreeWidget *personList=ui->personList;
    personList->setColumnWidth(0,150);
    personList->setIconSize(QSize(25,25));
    //分组
    QTreeWidgetItem *tag=new QTreeWidgetItem(personList);
    tag->setText(0,"分组1");
    //用户
    QTreeWidgetItem *person=new QTreeWidgetItem(tag);
    person->setText(0,"用户1");
    person->setIcon(0,QIcon(":/res/avatar.jpg"));

    //群聊页面
    QTreeWidget *groupList=ui->groupList;
    groupList->setIconSize(QSize(25,25));
    QTreeWidgetItem *group=new QTreeWidgetItem(groupList);
    group->setText(0,"群聊1");
    group->setIcon(0,QIcon(":/res/group.png"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
