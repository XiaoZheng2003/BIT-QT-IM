#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QElapsedTimer>
#include <QHash>
#include <QMutex>
#include "chat.h"

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QString ip,QWidget *parent = nullptr);
    ~File();
    void init();
    void refused();
    void setSocketDescriptor(qint64 socketDescriptor);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void sendFileName(QString);
    void startSend(QString fileName);
    void quit(QString ip,qint64 socketDescriptor);

public slots:
    void updateClientProgress(qint64 bytes,float timeUsed);
    void setTotalBytes(qint64 bytes);
    void sendFile();

private slots:
    void on_Button_choose_clicked();

    void on_Button_send_clicked();

    void on_Button_cancle_clicked();

private:
    Ui::File *ui;
    qint64 TotalBytes ;
    qint64 bytesWritten ;
    qint64 bytestoWrite;
    QString theFileName;
    QString fileName;
    QString ip;
    qintptr m_socketDescriptor;
};

class FileSender:public QObject
{
    Q_OBJECT
public:
    FileSender(qintptr socketDescriptor);

public slots:
    void startSend(QString fileName);
    void init();
    void quit();

private slots:
    void sendNext(qint64 bytes);

signals:
    void sendWrittenBytes(qint64 writtenBytes,float time);
    void sendTotalBytes(qint64 bytes);
    void startConnect(QString connectIp,qint64 socketDescriptor,FileSender *fileSender);

private:
    QTcpSocket *m_tcpSocket;
    QFile* m_localFile;
    qint64 m_socketDescriptor;
    qint64 m_totalBytes;
    qint64 m_writtenBytes;
    qint64 m_payloadSize;
    QString m_fileName;
    QElapsedTimer m_time;
    QByteArray m_outBlock;
};

class MyTcpServer:public QTcpServer
{
    Q_OBJECT
public:
    static void init(QObject *parent,qint16 tcpPort);
    static void addSendTarget(QString targetIp,Chat *chatWindow);
    static MyTcpServer *getInstance();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public slots:
    void startConnect(QString connectIp,qintptr socketDescriptor,FileSender *fileSender);
    void refuse(QString ip);

private slots:
    void deleteResourse(QString ip,qintptr socketDescriptor);

signals:
    void sendFileNameInfo(QString fileName);
    void startInit();
    void prepared();

private:
    static MyTcpServer* m_gMyTcpServer;
    static QMutex m_mutex;
    QHash<QString,File*> m_targetList;
    QHash<QString,FileSender*> m_fileSenderList;
    MyTcpServer(QObject* parent=nullptr);
    ~MyTcpServer();
    MyTcpServer(const MyTcpServer& ref);
    MyTcpServer& operator=(const MyTcpServer& ref);
};
#endif // FILE_H
