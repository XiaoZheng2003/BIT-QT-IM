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
    isGroup=0;
    on_search_clicked();
}

History::History(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);

    this->id=id;
    isGroup=1;
    on_search_clicked();
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
    if(!isGroup){
        query.exec("select * from person_msg where id="+QString::number(id)+" and msg like '%"+keyword+"%'");
        while(query.next()){
            tb->setCurrentFont(QFont("黑体",8));
            if(query.value(3).toInt()){
                tb->setTextColor(Qt::blue);
                tb->append(localName+"["+localIp+"]"+query.value(2).toString());
            }
            else{
                tb->setTextColor(Qt::green);
                tb->append(targetName+"["+targetIp+"]"+query.value(2).toString());
            }
            tb->append(query.value(1).toString());
        }
    }
    else{
        query.exec("select * from group_msg where id="+QString::number(id)+" and msg like '%"+keyword+"%'");
        while(query.next()){
            if(query.value(3).toInt())
                tb->setTextColor(Qt::blue);
            else
                tb->setTextColor(Qt::green);
            tb->append(query.value(4).toString()+" "+query.value(2).toString());
            tb->append(query.value(1).toString());
        }
    }
}
