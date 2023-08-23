#ifndef ADD_H
#define ADD_H

#include <QDialog>
#include <QMessageBox>

#include "networktool.h"

namespace Ui {
class Add;
}

class Add : public QDialog
{
    Q_OBJECT

public:
    explicit Add(QWidget *parent = nullptr);
    ~Add();

private slots:
    void on_personCancel_clicked();

    void on_groupCancel_clicked();

    void on_personTest_clicked();

    void on_groupTest_clicked();

    void on_personConfirm_clicked();

private:
    Ui::Add *ui;
};

#endif // ADD_H
