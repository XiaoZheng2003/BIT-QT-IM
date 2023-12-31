#include "login.h"
#include "dbmanager.h"
#include "threadmanager.h"
#include "file.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ThreadManager::init();

    MyTcpServer::init(nullptr,12689);

    // 打开数据库(如果不存在则创建后再打开)
    DBManager::init();
    // 创建表格
    DBManager::runSql("PRAGMA foreign_keys = ON");
    DBManager::runSql(QString("create table if not exists person"
                              " (id integer primary key, nickname varchar(50), ip varchar(20), avatar int DEFAULT (0))"));
    DBManager::runSql(QString("create table if not exists groups"
                              " (id integer primary key, groupname varchar(50), ips varchar(200))"));
    DBManager::runSql(QString("create table if not exists person_msg"
                              " (id integer REFERENCES person (id) ON DELETE CASCADE ON UPDATE CASCADE,"
                              " msg varchar(500), time timestamp, islocal boolean)"));
    DBManager::runSql(QString("create table if not exists group_msg"
                              " (id integer REFERENCES groups (id) ON DELETE CASCADE ON UPDATE CASCADE,"
                              " msg varchar(500), time timestamp, islocal boolean, ip varchar(20))"));
    DBManager::runSql("insert or ignore into person (id,nickname,avatar) values (-1,'智能聊天机器人',-1)");
    DBManager::runSql("insert or ignore into groups (id,groupname) values (-1,'公共聊天室')");

    Login lw;
    lw.show();
    return a.exec();
}
