#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Linpop");

    ui->ipAddress->setText("IP地址："+NetworkTool::GetLocalIP());

    QTreeWidget *personList=ui->personList;
    personList->setColumnWidth(0,150);
    personList->setIconSize(QSize(25,25));

    //分组
    QTreeWidgetItem *group=new QTreeWidgetItem(personList);
    group->setText(0,"分组1");

    //用户
    QTreeWidgetItem *person=new QTreeWidgetItem(group);
    person->setText(0,"用户1");
    person->setIcon(0,QIcon(":/res/avatar.jpg"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
