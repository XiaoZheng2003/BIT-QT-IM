#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "add.h"
#include "personedit.h"
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

private slots:
    void on_add_clicked();

public slots:
    void getUsername(QString);

private:
    Ui::MainWindow *ui;
    PersonEdit *m_personEditWindow;
    void refresh();
    void initMenu();
};

#endif // MAINWINDOW_H
