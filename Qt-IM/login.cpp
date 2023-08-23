#include "login.h"
#include "ui_login.h"
#include "QMessageBox"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}


void Login::on_in_btn_clicked()
{
    QString user;
       QString pwd;
       user = ui->username->text();//获取用户名
       pwd = ui->password->text();//获取密码
       //判断用户名密码是否为空，为空则提示不能为空
       if(user=="")
       QMessageBox::warning(this,"提示","用户名不能为空");
       else if (pwd=="")
       QMessageBox::warning(this,"提示","密码不能为空");
       else{
        QMessageBox::warning(this,"提示","登陆成功");
        this->close();
       }

}
