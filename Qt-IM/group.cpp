#include "group.h"
#include "ui_group.h"

Group::Group(HeartbeatBroadcaster * reci, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Group)
{
    ui->setupUi(this);
    receiver = reci;
    // 将信号 personListChanged() 与槽函数 refresh() 连接起来
    connect(receiver, SIGNAL(personListChanged()), this, SLOT(refresh()));
}

Group::~Group()
{
    delete ui;
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
        person->setIcon(0,QIcon(QString(":/res/avatar2.png")));
        person->setText(1,receiver->getReceivedIPs().at(i));
    }
}
