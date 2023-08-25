#ifndef NETWORKTOOL_H
#define NETWORKTOOL_H

#include <QString>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>

class NetworkTool
{
public:
    static QString GetLocalIP();
    static bool isIP(QString ip);
    static bool testPersonIPFormat(QString ip, QWidget *target);
    static bool testGroupIPFormat(QStringList ips, QWidget *target);
};

#endif // NETWORKTOOL_H
