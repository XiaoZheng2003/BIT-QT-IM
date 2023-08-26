#include "history.h"
#include "ui_history.h"

History::History(int id, QString lname, QString lip, QString tname, QString tip, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);

    this->id=id;
    localName=lname;
    localIp=lip;
    targetName=tname;
    targetIp=tip;
}

History::~History()
{
    delete ui;
}

void History::on_search_clicked()
{
    QString keyword=ui->keyword->text();
    QSqlQuery query;
    QTextBrowser *tb=ui->messageTextBrowser;
    tb->clear();
    query.exec("select * from msg where id="+QString::number(id)+" and msg like '%"+keyword+"%'");
    while(query.next()){
        tb->setCurrentFont(QFont("黑体",8));
        if(query.value(4).toInt()){
            tb->setTextColor(Qt::blue);
            tb->append(localName+"["+localIp+"]"+query.value(3).toString());
        }
        else{
            tb->setTextColor(Qt::green);
            tb->append(targetName+"["+targetIp+"]"+query.value(3).toString());
        }
        tb->append(query.value(2).toString());
    }
}
