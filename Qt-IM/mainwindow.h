#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QUdpSocket>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "add.h"
#include "chat.h"
#include "edit.h"
#include "avatar.h"
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
    void receiveMsg();

private slots:
    void on_add_clicked();
    void processPendinDatagrams();

    void on_avatar_clicked();

//protected:
//    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
    QUdpSocket *xchat;
    qint32 xport;
    QString localName;
    QString localIp;
    Chat *chat;
    Edit *m_editWindow;
    int avatarId;
    Avatar a;

    void refresh();
    void initMenu();
    void updateOnline();
    void hasPendinFile(QString serverAddress,QString clientAddress,QString filename);
};

#endif // MAINWINDOW_H
