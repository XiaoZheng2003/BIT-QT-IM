#include "login.h"
#include "ui_login.h"
#include "regist.h"

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

void Login::on_regist_clicked()
{
    this->close();
    Regist *r=new Regist;
    r->show();

}
