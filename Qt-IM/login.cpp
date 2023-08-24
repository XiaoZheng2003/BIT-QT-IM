#include "login.h"
#include "ui_login.h"
#include "QMessageBox"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");
    connect(this,&Login::sendUsername,&mw,&MainWindow::getUsername);

    QIcon icon(":/res/avatar.jpg");
    ui->pic->setIcon(icon);//登录页面设置图片

}

Login::~Login()
{
    delete ui;
}


void Login::on_in_btn_clicked()
{
    QString user;
    user = ui->username->text();//获取用户名
    if(user=="")
        QMessageBox::critical(this,"提示","用户名不能为空");
    else{
        QMessageBox::information(this,"提示","登录成功");
        emit sendUsername(user);
        mw.show();
        this->close();
    }


}
