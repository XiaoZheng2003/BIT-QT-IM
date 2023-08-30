#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QDialog>
#include <QDateTime>
#include <QKeyEvent>
#include <QUdpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QListWidgetItem>

#include "dbmanager.h"
#include "networktool.h"
#include "filerec.h"
#include "history.h"
#include "emoji.h"
#include "chatmessage.h"

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(int laid, int taid, QString ip,int tid,
                  QString tname,QString tip,QUdpSocket *xchat,qint32 xport,
                  QString lname,QWidget *parent = nullptr);
    ~Chat();

    void refresh();
    void sendMessage(messageType type,QString serverAddress = "");
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);


protected:
    void keyPressEvent(QKeyEvent *ev);
    void resizeEvent(QResizeEvent *event);

public slots:
    void getSendFileName(QString);

private slots:
    void on_sendMsg_clicked();

    void on_file_clicked();

    void on_closeBtn_clicked();

    void on_history_clicked();

    void on_emote_clicked();

    void chatRobot();

private:
    Ui::Chat *ui;
    int targetId;
    int localAvatarId;
    int targetAvatarId;
    QString localName;
    QString localIp;
    QString targetName;
    QString targetIp;
    QString message;
    QUdpSocket *xchat;
    qint32 xport;
    QString fileName;
    History *history;
    Emoji *emoji;

    QString getMessage();

};

#endif // CHAT_H
