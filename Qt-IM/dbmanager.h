#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

class DBManager
{
public:
    static bool init(QString dbName = QString("localdb.db"));

    static bool runSql(QString sql);
private:
    static DBManager *m_gDBMangaer;
    DBManager(QString dbName);

    QSqlDatabase m_database;
};

#endif // DBMANAGER_H
