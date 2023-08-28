#ifndef NETWORKTOOL_H
#define NETWORKTOOL_H

#include <QtSql>
#include <QString>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>

//信息类型,分别为普通消息,新加入用户,用户退出,发送文件,拒绝接受文件,发起私聊
enum messageType{
    PersonMessage,
    GroupMessage,
    NewParticipant,
    LeftParticipant,
    SendFileName,
    RefuseFile,
    AskChat
};

class NetworkTool
{
public:
    static QString GetLocalIP();
    static bool isIP(QString ip);
    static bool testPersonIPFormat(QString ip, QWidget *target);
    static bool testGroupIPFormat(QStringList ips, QWidget *target);
    static bool isIpExist(QString ip);
};

#endif // NETWORKTOOL_H
