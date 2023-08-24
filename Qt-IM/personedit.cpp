#include "personedit.h"
#include "ui_personedit.h"

PersonEdit::PersonEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonEdit)
{
    ui->setupUi(this);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=](){
        QString nickname=ui->remark->text();
        QString personIP=ui->personIP->text();
        if(testPersonIPFormat(personIP))
        {
            DBManager::runSql(QString("update person set nickname='%1', ip='%2' where id=%3").arg(nickname).arg(personIP).arg(m_id));
            this->close();
        }
    });
}

PersonEdit::~PersonEdit()
{
    delete ui;
}

void PersonEdit::init(int id, bool isGroup)
{
    std::vector<std::vector<QString>> result;
    if(!isGroup)
        DBManager::runSql(QString("select nickname,ip from person where id=%1").arg(id),result);
    else
        DBManager::runSql(QString("select groupname,ips from groups where id=%1").arg(id),result);
    m_nicknameBefore=result[0][0];
    m_ipBefore=result[0][1];
    m_id=id;
    ui->remark->setText(m_nicknameBefore);
    ui->personIP->setText(m_ipBefore);
}

bool PersonEdit::testPersonIPFormat(QString ip)
{
    //测试好友IP地址格式是否正确
    if(!NetworkTool::isIP(ip)){
        QMessageBox::critical(this,tr("错误"),tr("IP地址不合法！"));
        return false;
    }
    return true;
}
