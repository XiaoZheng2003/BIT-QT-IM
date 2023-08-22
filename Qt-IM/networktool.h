#ifndef NETWORKTOOL_H
#define NETWORKTOOL_H

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>

class NetworkTool
{
public:
    static bool IsIP(const QString &ip);
    static QString GetLocalIP();

};

#endif // NETWORKTOOL_H
