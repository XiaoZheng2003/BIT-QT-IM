#include "regist.h"
#include "ui_regist.h"
#include "login.h"

regist::regist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regist)
{
    ui->setupUi(this);
}

regist::~regist()
{
    delete ui;
}


void regist::on_in_btn_clicked()
{

        this->close();
        login *l=new login;
        l->show();

}
