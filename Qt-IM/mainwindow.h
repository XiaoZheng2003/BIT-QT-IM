#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QVector>
#include <QUdpSocket>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "add.h"
#include "chat.h"
#include "edit.h"
#include "avatar.h"
#include "file.h"
#include "dbmanager.h"
#include "networktool.h"
#include "heartbeatbroadcaster.h"
#include "heartbeatreceiver.h"
#include "group.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QString ip, QString username, QWidget *parent = nullptr);
    ~MainWindow();
    HeartbeatBroadcaster* broadcaster;
    HeartbeatReceiver* receiver;

signals:
    void receivePersonMsg();
    void receiveGroupMsg();
    void refuse(QString ip);

private slots:
    void on_add_clicked();
    void processPendinDatagrams();

    void on_avatar_clicked();

//protected:
//    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
    QUdpSocket *xchat;
    QUdpSocket *groupxchat;
    qint32 xport;
    QString localName;
    QString localIp;
    Chat *chat;
    Group *group;  
    Edit *m_editWindow;
    int avatarId;
    Avatar a;
    QVector<int> unreadMsg;

    void refresh();
    void initMenu();
    void updateOnline();
    void hasPendinFile(QString serverAddress,QString clientAddress,QString filename);
    void groupProcessPendinDatagrams();
    void readMsg(int id);
};

#endif // MAINWINDOW_H
