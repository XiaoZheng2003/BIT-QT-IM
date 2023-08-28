#include "login.h"
#include "ui_login.h"
#include "QMessageBox"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");

    QPixmap *pixmap=new QPixmap(":/res/banner.jpg");
    pixmap->scaled(ui->banner->size(),Qt::KeepAspectRatio);
    ui->banner->setScaledContents(true);
    ui->banner->setPixmap(*pixmap);
}

Login::~Login()
{
    delete ui;
}


void Login::on_login_clicked()
{
    QString user;
    user = ui->username->text();//获取用户名
    if(user=="")
        QMessageBox::critical(this,"提示","用户名不能为空");
    else{
        QMessageBox::information(this,"提示","登录成功");
        emit sendUsername(user);
        mw = new MainWindow;
        connect(this,&Login::sendUsername,mw,&MainWindow::getUsername);
        mw->show();
        this->close();
    }


}
