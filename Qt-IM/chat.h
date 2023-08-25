#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDateTime>
#include <QUdpSocket>
#include <QMessageBox>

#include "dbmanager.h"
#include "networktool.h"

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(bool isGroup,int tid,QString tname,QString tip,QUdpSocket *xchat,qint32 xport,QString lname,QWidget *parent = nullptr);
    ~Chat();

    void refresh();

private slots:
    void on_sendMsg_clicked();

private:
    Ui::Chat *ui;
    int targetId;
    QString localName;
    QString localIp;
    QString targetName;
    QString targetIp;
    QString message;
    QUdpSocket *xchat;
    qint32 xport;
    bool isGroup;

    QString getMessage();
    void sendMessage(messageType type,QString serverAddress = "");
};

#endif // CHAT_H
