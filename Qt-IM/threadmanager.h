#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QDebug>
#include <QThread>
#include <QHash>
#include <QWidget>
#include <QMutex>

class ThreadManager
{
public:
    static void init();

    static bool addObject(QString name,QObject *object);//将一个QWidget对象添加到名称对应的Thread中，返回true代表id对应的Thread之前不存在，返回false代表id对应的Thread已经存在

    static bool deleteObject(QString name,QObject *object);//删除指定名称线程中的QWidget对象，成功返回true，失败返回false

    static bool deleteThread(QString name);//删除指定名称的线程，成功返回true，失败返回false

    static bool startThread(QString name);//开启指定名称线程，成功返回true，失败返回false

private:
    QHash<QString,QThread *> m_threadList;
    static QMutex m_mutex;

    ThreadManager();
    ~ThreadManager();
    ThreadManager(const ThreadManager& ref);
    ThreadManager& operator=(const ThreadManager& ref);


    static ThreadManager *m_gThreadManager;
};

#endif // THREADMANAGER_H
