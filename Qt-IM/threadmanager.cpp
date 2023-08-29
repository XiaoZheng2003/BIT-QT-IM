#include "threadmanager.h"
ThreadManager *ThreadManager::m_gThreadManager=nullptr;
QMutex ThreadManager::m_mutex;

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
    QMutexLocker locker(&m_mutex);
    QHash<QString,QThread*>::const_iterator iterator=m_gThreadManager->m_threadList.constBegin();
    while(iterator!=m_gThreadManager->m_threadList.constEnd())
    {
        iterator.value()->quit();
        iterator.value()->wait();
        iterator++;
    }
}

void  ThreadManager::init()
{
    if(m_gThreadManager==nullptr)
    {
        QMutexLocker locker(&m_mutex);
        if(m_gThreadManager==nullptr)
            m_gThreadManager=new ThreadManager;
    }
}

bool ThreadManager::addObject(QString name, QObject *object)
{
    QMutexLocker locker(&m_mutex);
    if(m_gThreadManager->m_threadList.contains(name))
    {
        object->moveToThread(m_gThreadManager->m_threadList.value(name));
        return false;
    }
    else
    {
        QThread *newThread=new QThread();
        m_gThreadManager->m_threadList.insert(name,newThread);
        object->moveToThread(m_gThreadManager->m_threadList.value(name));
        return true;
    }
}

bool ThreadManager::deleteObject(QString name, QObject *object)
{
    QMutexLocker locker(&m_mutex);
    if(m_gThreadManager->m_threadList.contains(name))
    {
        object->moveToThread(nullptr);
        return true;
    }
    else
    {
        return false;
    }
}

bool ThreadManager::deleteThread(QString name)
{
    QMutexLocker locker(&m_mutex);
    if(m_gThreadManager->m_threadList.contains(name))
    {
        m_gThreadManager->m_threadList.value(name)->terminate();
        m_gThreadManager->m_threadList.value(name)->wait();
        m_gThreadManager->m_threadList.value(name)->deleteLater();
        m_gThreadManager->m_threadList.remove(name);
        return true;
    }
    else
    {
        return false;
    }
}

bool ThreadManager::startThread(QString name)
{
    QMutexLocker locker(&m_mutex);
    if(m_gThreadManager->m_threadList.contains(name))
    {
        m_gThreadManager->m_threadList.value(name)->start();
        return true;
    }
    else
    {
        return false;
    }
}
