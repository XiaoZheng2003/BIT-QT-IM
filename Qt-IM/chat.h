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
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();

    void init(bool isGroup,int tid,QString tname,QString tip,QUdpSocket *xchat,qint32 xport);
    void refresh();

private slots:
    void on_sendMsg_clicked();

private:
    Ui::Chat *ui;
    int targetId;
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
