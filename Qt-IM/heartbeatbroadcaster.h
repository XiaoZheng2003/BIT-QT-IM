#ifndef HEARTBEATBROADCASTER_H
#define HEARTBEATBROADCASTER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class HeartbeatBroadcaster : public QObject
{
    Q_OBJECT

public:
    HeartbeatBroadcaster(QObject *parent = nullptr);
    QList<QString> getReceivedIPs();

signals:
    void personListChanged(); // 自定义信号，表示人员列表发生变化

private slots:
    void sendHeartbeat();
    void readPendingDatagrams();

private:
    QUdpSocket *socket;
    QTimer *heartbeatTimer;
    QList<QString> receivedIPs;
};

#endif // HEARTBEATBROADCASTER_H
