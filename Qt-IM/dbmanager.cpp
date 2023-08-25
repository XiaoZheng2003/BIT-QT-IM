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

bool DBManager::runSql(QString sql, std::vector<std::vector<QString>>& result)
{
    QSqlQuery query;
    std::vector<std::vector<QString>> temp;
    if(query.exec(sql) == false) {
        qCritical()<<"Fail to run sql: "<<sql<<" | "<<m_gDBMangaer->m_database.lastError().text();
        return false;
    } else {

        QSqlRecord rec=query.record();
        int row=0;
        while(query.next())
        {
            std::vector<QString> tempRow(rec.count(),QString(""));
            temp.push_back(tempRow);
            for(int col=0;col<rec.count();col++)
            {
                temp[row][col]=query.value(col).toString();
            }
            row++;
        }
        qDebug()<<"run sql successfully."<<sql;
        result=temp;
        return true;
    }
}
