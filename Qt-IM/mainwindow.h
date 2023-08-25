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

public slots:
    void getUsername(QString);

private:
    Ui::MainWindow *ui;
    QUdpSocket *xchat;
    qint32 xport;
    QString localIp;
    Chat chat;

    void refresh();
    void initMenu();
};

#endif // MAINWINDOW_H
