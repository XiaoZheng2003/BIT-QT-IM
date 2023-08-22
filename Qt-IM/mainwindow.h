#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
