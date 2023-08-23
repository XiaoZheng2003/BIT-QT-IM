#include "regist.h"
#include "ui_regist.h"
#include "login.h"

Regist::Regist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regist)
{
    ui->setupUi(this);
}

Regist::~Regist()
{
    delete ui;
}


void Regist::on_in_btn_clicked()
{

        this->close();
        Login *l=new Login;
        l->show();

}
