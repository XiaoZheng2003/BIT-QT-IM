#ifndef HEARTBEATRECEIVER_H
#define HEARTBEATRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include "networktool.h"

class HeartbeatReceiver : public QObject
{
    Q_OBJECT

public:
    HeartbeatReceiver(QString localIP, QObject *parent = nullptr);

    QStringList onlineIPs;

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *socket;
    QString lIP;

};

#endif // HEARTBEATRECEIVER_H
