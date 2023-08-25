#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDialog>
#include <QDateTime>
#include <QUdpSocket>
#include <QMessageBox>
#include <QFileDialog>

#include "dbmanager.h"
#include "networktool.h"
#include "file.h"
#include "filerec.h"

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

    void on_pushButton_3_clicked();

    void getSendFileName(QString);

private:
    Ui::Chat *ui;
    int targetId;
    QString localIp;
    QString targetName;
    QString targetIp;
    QString message;
    QUdpSocket *xchat;
    qint32 xport;
    File server;
    QString fileName;
    bool isGroup;

    QString getMessage();
    void sendMessage(messageType type,QString serverAddress = "");

};

#endif // CHAT_H
