#include "heartbeatbroadcaster.h"
#include <QDebug>

HeartbeatBroadcaster::HeartbeatBroadcaster(QObject *parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);
    sendHeartbeat();
    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, &QTimer::timeout, this, &HeartbeatBroadcaster::sendHeartbeat);
    heartbeatTimer->start(10000); // 每10秒发送一次心跳包

    connect(socket, &QUdpSocket::readyRead, this, &HeartbeatBroadcaster::readPendingDatagrams);
}

void HeartbeatBroadcaster::sendHeartbeat()
{
    QList<QString> preReceivedIPs = receivedIPs;
    receivedIPs.clear(); // 清空已收到的IP列表
    QByteArray datagram = "heartbeat"; // 心跳包内容
    socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 12345); // 广播心跳包
    // 在心跳包发送后等待一段时间以确保所有回复都已经收到
    QTimer::singleShot(2000, this, [=]() {
        if(preReceivedIPs!=receivedIPs)
            emit personListChanged(); // 发送更新信号
    });
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
//                qDebug() << "Received IPs:" << receivedIPs;
            }
        }
    }
}

QList<QString> HeartbeatBroadcaster::getReceivedIPs(){
    return receivedIPs;
}

