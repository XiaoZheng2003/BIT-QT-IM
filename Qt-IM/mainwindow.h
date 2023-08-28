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

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void receiveMsg();

private slots:
    void on_add_clicked();
    void processPendinDatagrams();

    void on_avatar_clicked();

public slots:
    void getUsername(QString);

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

    void refresh();
    void initMenu();
    void hasPendinFile(QString serverAddress,QString clientAddress,QString filename);
};

#endif // MAINWINDOW_H
