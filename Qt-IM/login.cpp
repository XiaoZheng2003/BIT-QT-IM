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

    ip=NetworkTool::GetLocalIP();
    if(ip.contains("|")){
        IpSelect *is=new IpSelect();
        is->setAttribute(Qt::WA_DeleteOnClose);
        connect(is,&IpSelect::selectedIp,[=](QString ip){
            this->ip=ip;
        });
        is->exec();
    }
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
        mw = new MainWindow(ip, user);
        mw->show();
        this->close();
    }


}
