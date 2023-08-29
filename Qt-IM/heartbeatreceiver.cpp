#include "heartbeatreceiver.h"

#include <QDebug>

HeartbeatReceiver::HeartbeatReceiver(QObject *parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(12345, QUdpSocket::ShareAddress); // 监听广播端口

    connect(socket, &QUdpSocket::readyRead, this, &HeartbeatReceiver::readPendingDatagrams);
}

void HeartbeatReceiver::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        QString message = datagram.data();
        if (message == "heartbeat")
        {
            QString localIP = NetworkTool::GetLocalIP();
            // 收到心跳包，回复IP地址
            QByteArray response = "response"+ localIP.toUtf8();
            socket->writeDatagram(response, response.size(), senderAddress, senderPort);

            QString senderIP = senderAddress.toString();
            qDebug() << "Online IPs:" << localIP;

        }
    }
}

