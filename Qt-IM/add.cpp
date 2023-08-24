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
    if(testPersonIPFormat(ui->personIP->text())){  //IP格式正确
        QMessageBox::information(this,tr("信息"),tr("IP地址格式正确，连通性待测试"));
        //TODO: 测试连通性
    }
}

void Add::on_groupTest_clicked()
{
    //添加群聊测试连通性
    if(testGroupIPFormat(ui->groupIP->toPlainText())){  //IP格式正确
        QMessageBox::information(this,tr("信息"),tr("IP地址格式正确，连通性待测试"));
        //TODO: 测试连通性
    }
}

void Add::on_personConfirm_clicked()
{
    //添加好友
    if(!testPersonIPFormat(ui->personIP->text()))   //IP格式错误
        return;
    //TODO: 添加好友（数据库）
}

void Add::on_groupConfirm_clicked()
{
    //添加群聊
    if(!testGroupIPFormat(ui->groupIP->toPlainText()))  //IP格式错误
        return;
    //TODO: 添加群聊（数据库）
}

bool Add::testPersonIPFormat(QString ip)
{
    //测试好友IP地址格式是否正确
    if(!NetworkTool::isIP(ip)){
        QMessageBox::critical(this,tr("错误"),tr("IP地址不合法！"));
        return false;
    }
    return true;
}

bool Add::testGroupIPFormat(QString ips)
{
    //测试群聊IP地址格式是否正确
    QStringList ls=ips.split("\n");
    bool isEmpty=true;  //当前IP列表为空
    for(QString ip:ls){
        if(ip=="") continue;    //忽略空行
        isEmpty=false;
        if(!NetworkTool::isIP(ip)){
            QMessageBox::critical(this,tr("错误"),tr("IP地址不合法！"));
            return false;
        }
    }
    if(isEmpty){
        QMessageBox::critical(this,tr("错误"),tr("IP列表为空！"));
        return false;
    }
    return true;
}
