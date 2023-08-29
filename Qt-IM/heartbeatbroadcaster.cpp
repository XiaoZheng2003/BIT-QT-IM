#include "heartbeatbroadcaster.h"
#include <QDebug>

HeartbeatBroadcaster::HeartbeatBroadcaster(QObject *parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);
    sendHeartbeat();
    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, &QTimer::timeout, this, &HeartbeatBroadcaster::sendHeartbeat);
    heartbeatTimer->start(5000); // 每5秒发送一次心跳包

    connect(socket, &QUdpSocket::readyRead, this, &HeartbeatBroadcaster::readPendingDatagrams);
}

void HeartbeatBroadcaster::sendHeartbeat()
{
    receivedIPs.clear(); // 清空已收到的IP列表
    QByteArray datagram = "heartbeat"; // 心跳包内容
    socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 12345); // 广播心跳包
}

void HeartbeatBroadcaster::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        QString message = datagram.data();
        if (message.left(8) == "response")
        {
            QString senderIP = message.mid(8);
            if (!receivedIPs.contains(senderIP))
            {
                receivedIPs.append(senderIP);
                qDebug() << "Received IPs:" << receivedIPs;
            }
        }
    }
}

QList<QString> HeartbeatBroadcaster::getReceivedIPs(){
    return receivedIPs;
}

