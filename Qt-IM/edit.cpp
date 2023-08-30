#include "edit.h"
#include "ui_edit.h"

Edit::Edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);
}

Edit::~Edit()
{
    delete ui;
}

void Edit::init(int id, bool isGroup)
{
    QList<QStringList> result;
    m_isGroup=isGroup;
    if(!isGroup)
        result=DBManager::runSelect(QString("select nickname,ip from person where id=%1").arg(id));
    else{
        result=DBManager::runSelect(QString("select groupname,ips from groups where id=%1").arg(id));
        ui->label_name->setText("群聊名称");
    }
    m_nicknameBefore=result[0][0];
    m_ipBefore=result[1][0];
    m_id=id;
    ui->remark->setText(m_nicknameBefore);
    ui->IP->setText(m_ipBefore);
}

void Edit::on_buttonBox_accepted()
{
    QString nickname=ui->remark->text();
    QString IP=ui->IP->text();
    if(!m_isGroup)
    {
        if(m_ipBefore!=IP&&NetworkTool::isIpExist(IP)){
            QMessageBox::critical(this,tr("错误"),tr("该IP已存在！"));
            return;
        }
        if(NetworkTool::testPersonIPFormat(IP,this))
        {
            DBManager::runSql(QString("update person set nickname='%1', ip='%2' where id=%3").arg(nickname).arg(IP).arg(m_id));
            this->close();
        }
    }
    else
    {
        if(NetworkTool::testGroupIPFormat(IP.split(';'),this))
        {
            DBManager::runSql(QString("update groups set groupname='%1', ips='%2' where id=%3").arg(nickname).arg(IP).arg(m_id));
            this->close();
        }
    }
}


void Edit::on_buttonBox_rejected()
{
    this->close();
}

