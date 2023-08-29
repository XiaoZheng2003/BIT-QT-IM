#ifndef HEARTBEATRECEIVER_H
#define HEARTBEATRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include "networktool.h"

class HeartbeatReceiver : public QObject
{
    Q_OBJECT

public:
    HeartbeatReceiver(QObject *parent = nullptr);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *socket;
    QList<QString> onlineIPs;
};

#endif // HEARTBEATRECEIVER_H
