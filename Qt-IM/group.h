#ifndef GROUP_H
#define GROUP_H

#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>

#include "emoji.h"
#include "history.h"
#include "dbmanager.h"
#include "networktool.h"
#include "heartbeatbroadcaster.h"

namespace Ui {
class Group;
}

class Group : public QDialog
{
    Q_OBJECT

public:
    explicit Group(HeartbeatBroadcaster * reci, QString ip, QWidget *parent = nullptr);
    ~Group();

    void reloadMessage();

protected:
    void keyPressEvent(QKeyEvent *ev);

private:
    Ui::Group *ui;
    HeartbeatBroadcaster * receiver;
    QUdpSocket *xchat;
    QString localIp;
    qint32 xport;
    int groupId;
    Emoji *emoji;
    History *history;

    void initMenu();
    void sendMessage(messageType type);
    QString getMessage();

signals:
    void addPerson();

private slots:
    void refresh();
    void on_closeBtn_clicked();
    void on_sendMsg_clicked();
    void on_emote_clicked();
    void on_history_clicked();
};

#endif // GROUP_H
