#ifndef NETWORKTOOL_H
#define NETWORKTOOL_H

#include <QString>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>

class NetworkTool
{
public:
    static QString GetLocalIP();
    static bool isIP(QString ip);

};

#endif // NETWORKTOOL_H
