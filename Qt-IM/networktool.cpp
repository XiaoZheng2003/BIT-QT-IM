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

bool NetworkTool::testPersonIPFormat(QString ip, QWidget *target)
{
    //测试好友IP地址格式是否正确
    if(!isIP(ip)){
        QMessageBox::critical(target,"错误","IP地址不合法！");
        return false;
    }
    return true;
}

bool NetworkTool::testGroupIPFormat(QStringList ips, QWidget *target)
{
    //测试群聊IP地址格式是否正确
    bool isEmpty=true;  //当前IP列表为空
    for(QString ip:ips){
        if(ip=="") continue;    //忽略空行
        isEmpty=false;
        if(!NetworkTool::isIP(ip)){
            QMessageBox::critical(target,"错误","IP地址不合法！");
            return false;
        }
    }
    if(isEmpty){
        QMessageBox::critical(target,"错误","IP列表为空！");
        return false;
    }
    return true;
}
