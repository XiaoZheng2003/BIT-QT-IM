#include "login.h"
#include "ui_login.h"
#include "regist.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_regist_clicked()
{
    this->close();
    regist *r=new regist;
    r->show();

}
