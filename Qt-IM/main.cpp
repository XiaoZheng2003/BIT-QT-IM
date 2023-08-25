#include "login.h"
#include "dbmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 打开数据库(如果不存在则创建后再打开)
    DBManager::init();
    // 创建表格
    DBManager::runSql("create table if not exists person (id integer primary key, nickname varchar(50), ip varchar(20))");
    DBManager::runSql("create table if not exists groups (id integer primary key, groupname varchar(50), ips varchar(200), num int)");
    DBManager::runSql("create table if not exists msg(type boolean, id integer, msg varchar(200), time timestamp, islocal boolean);");

    Login lw;
    lw.show();
    return a.exec();
}
