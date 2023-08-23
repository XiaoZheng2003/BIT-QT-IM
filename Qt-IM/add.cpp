#include "add.h"
#include "ui_add.h"

Add::Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add)
{
    ui->setupUi(this);
    this->setWindowTitle("添加好友/群");
}

Add::~Add()
{
    delete ui;
}

void Add::on_personCancel_clicked()
{
    this->close();
}

void Add::on_groupCancel_clicked()
{
    this->close();
}

void Add::on_personTest_clicked()
{
    //添加好友测试连通性
    QString ip=ui->personIP->text();
    if(!NetworkTool::isIP(ip)){
        QMessageBox::critical(this,tr("错误"),tr("IP地址不合法！"));
        return;
    }
    QMessageBox::information(this,tr("信息"),tr("IP地址格式正确，连通性待测试"));
}

void Add::on_groupTest_clicked()
{
    //添加群聊测试连通性
    QStringList ips=ui->GroupIP->toPlainText().split("\n");
    for(QString ip:ips){
        if(!NetworkTool::isIP(ip)){
            QMessageBox::critical(this,tr("错误"),tr("IP地址不合法！"));
            return;
        }
    }
    QMessageBox::information(this,tr("信息"),tr("IP地址格式正确，连通性待测试"));
}

void Add::on_personConfirm_clicked()
{
    //添加好友
    QString ip=ui->personIP->text();
    if(!NetworkTool::isIP(ip)){
        QMessageBox::critical(this,tr("错误"),tr("IP地址不合法！"));
        return;
    }
    //TODO: 添加好友（数据库）
}
