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

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *socket;
    QList<QString> onlineIPs;
    QString lIP;

};

#endif // HEARTBEATRECEIVER_H
