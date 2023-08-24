#include "networktool.h"

QString NetworkTool::GetLocalIP()
{
    //获取IP地址
    QStringList ips;
    QList<QNetworkInterface> networkinterfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, networkinterfaces)
    {
        QNetworkInterface::InterfaceFlags t_interFlags =  interface.flags();
        if(t_interFlags & QNetworkInterface::IsUp &&
            t_interFlags & QNetworkInterface::IsRunning &&
            !interface.hardwareAddress().startsWith("00:50:56")
            && !interface.hardwareAddress().startsWith("0A:00:27")
            && !interface.humanReadableName().contains("Virtual")
            && !interface.humanReadableName().contains("VM"))
        {
            foreach (QNetworkAddressEntry entry, interface.addressEntries())
            {
                if (entry.ip() != QHostAddress::LocalHost
                    && entry.ip().protocol() == QAbstractSocket::IPv4Protocol
                    )
                {
                    ips.append(entry.ip().toString());
                }
            }
        }
    }
    return ips.join("|");
}

bool NetworkTool::isIP(QString ip)
{
    if(ip.isEmpty()) return false;
    QStringList ls=ip.split('.');
    if(ls.size()!=4) return false;
    for(const auto &num:ls){
        bool ok=false;
        int tmp=num.toInt(&ok);
        if(!ok||tmp<0||tmp>255)
            return false;
    }
    return true;
}
