#include "avatar.h"
#include "ui_avatar.h"

Avatar::Avatar(int aid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Avatar)
{
    ui->setupUi(this);

    avatarId=aid;
    QToolButton *cur;
    switch(avatarId){
    case 0:cur=ui->avatar0;break;
    case 1:cur=ui->avatar1;break;
    case 2:cur=ui->avatar2;break;
    case 3:cur=ui->avatar3;break;
    case 4:cur=ui->avatar4;break;
    case 5:cur=ui->avatar5;break;
    }
    cur->setChecked(true);
}

Avatar::~Avatar()
{
    delete ui;
}

void Avatar::on_avatar0_clicked()
{
    avatarId=0;
}

void Avatar::on_avatar1_clicked()
{
    avatarId=1;
}

void Avatar::on_avatar2_clicked()
{
    avatarId=2;
}

void Avatar::on_avatar3_clicked()
{
    avatarId=3;
}

void Avatar::on_avatar4_clicked()
{
    avatarId=4;
}

void Avatar::on_avatar5_clicked()
{
    avatarId=5;
}

void Avatar::on_buttonBox_accepted()
{
    emit changeAvatarId(avatarId);
}
