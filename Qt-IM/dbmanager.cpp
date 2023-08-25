#include "dbmanager.h"

DBManager *DBManager::m_gDBMangaer = NULL;

DBManager::DBManager(QString dbName)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE"); // 使用sqlite数据库
    m_database.setDatabaseName(dbName); // 数据库名称

    if(!m_database.open()) { // 如果数据库不存在则创建并打开，如果已存在则打开
        qCritical()<<"Fail to create or connect database. "<<m_database.lastError().text();
    } else {
        qDebug()<<"Success to connect local database. "<<dbName;
    }
}

bool DBManager::init(QString dbName) {
    if(m_gDBMangaer == NULL) {
        m_gDBMangaer = new DBManager(dbName);
    }

    if(m_gDBMangaer->m_database.isOpen())
        return true;
    else
        return false;
}

bool DBManager::runSql(QString sql) {
    QSqlQuery query;

    if(query.exec(sql) == false) {
        qCritical()<<"Fail to run sql: "<<sql<<" | "<<m_gDBMangaer->m_database.lastError().text();
        return false;
    } else {
        qDebug()<<"run sql successfully. "<<sql;
        return true;
    }
}

//返回按列主序储存的查询结果
QList<QStringList> DBManager::runSelect(QString sql)
{
    QSqlQuery query;
    QList<QStringList> result;
    if(query.exec(sql) == false) {
        qCritical()<<"Fail to run sql: "<<sql<<" | "<<m_gDBMangaer->m_database.lastError().text();
    } else {

        QSqlRecord rec=query.record();
        for(int col=0;col<rec.count();col++)
        {
            QStringList temp;
            result.append(temp);
        }
        while(query.next())
        {
            for(int col=0;col<rec.count();col++)
            {
                result[col].append(query.value(col).toString());
            }
        }
        qDebug()<<"run sql successfully."<<sql;
        return result;
    }
}
