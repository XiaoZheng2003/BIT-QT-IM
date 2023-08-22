#include "networktool.h"

bool NetworkTool::IsIP(const QString &ip) {
    //判断是否为IP
    QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.)"
                   "{3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    return RegExp.exactMatch(ip);
}

QString NetworkTool::GetLocalIP()
{
    //获取本地ip地址
    QStringList ips;
    QList<QHostAddress> addrs = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, addrs) {
        QString ip = addr.toString();
        if (IsIP(ip)) {
            ips << ip;
        }
    }
    //优先192,无则127.0.0.1
    QString ip = "127.0.0.1";
    foreach (QString str, ips) {
        if (!str.startsWith("127")) {
            ip = str;
            break;
        }
    }
    return ip;
}
