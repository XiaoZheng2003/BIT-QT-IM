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
           t_interFlags & QNetworkInterface::IsRunning)
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
