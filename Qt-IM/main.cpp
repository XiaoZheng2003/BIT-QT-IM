#include "login.h"
#include "dbmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 打开数据库(如果不存在则创建后再打开)
    DBManager::init();
    // 创建表格
    DBManager::runSql("delete from group");
    DBManager::runSql("create table if not exists person (id integer primary key, nickname varchar(50), ip varchar(20))");
    DBManager::runSql("create table if not exists groups (id integer primary key, groupname varchar(50), ips varchar(200), num int)");

    Login lw;
    lw.show();
    return a.exec();
}
