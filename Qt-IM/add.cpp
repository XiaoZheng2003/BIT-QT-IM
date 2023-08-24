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
    if(testGroupIPFormat(ui->groupIP->toPlainText().split("\n"))){  //IP格式正确
        QMessageBox::information(this,tr("信息"),tr("IP地址格式正确，连通性待测试"));
        //TODO: 测试连通性
    }
}

void Add::on_personConfirm_clicked()
{
    //添加好友
    QString nickname=ui->nickname->text();
    QString ip=ui->personIP->text();
    if(!testPersonIPFormat(ip))   //IP格式错误
        return;
    //添加好友（数据库）
    DBManager::runSql(QString("insert into person (nickname,ip) values('%1', '%2')").arg(nickname).arg(ip));
    this->close();
}

void Add::on_groupConfirm_clicked()
{
    //添加群聊
    QString groupname=ui->groupName->text();
    QStringList ips=ui->groupIP->toPlainText().split("\n");
    ips.removeAll(QString(""));
    if(!testGroupIPFormat(ips))  //IP格式错误
        return;
    //添加群聊（数据库）
    DBManager::runSql(QString("insert into groups (groupname,ips,num) values('%1', '%2', %3)").arg(groupname).arg(ips.join(';')).arg(ips.count()));
    this->close();
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

bool Add::testGroupIPFormat(QStringList ips)
{
    //测试群聊IP地址格式是否正确
    bool isEmpty=true;  //当前IP列表为空
    for(QString ip:ips){
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
