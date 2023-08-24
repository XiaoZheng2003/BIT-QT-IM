#include "personedit.h"
#include "ui_personedit.h"

PersonEdit::PersonEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonEdit)
{
    ui->setupUi(this);
}

PersonEdit::~PersonEdit()
{
    delete ui;
}

void PersonEdit::init(QString nickname, QString ip)
{
    m_nicknameBefore=nickname;
    m_ipBefore=ip;
    ui->nickname->setText(nickname);
    ui->personIP->setText(ip);
}

void PersonEdit::commitChange()
{
    QString nickname=ui->nickname->text();
    QString personIP=ui->personIP->text();
    DBManager::runSql(QString("update person set nickname='%1', ip='%2' where nickname='%3' and ip='%4'").arg(nickname).arg(personIP).arg(m_nicknameBefore).arg(m_ipBefore));
    this->close();
}
