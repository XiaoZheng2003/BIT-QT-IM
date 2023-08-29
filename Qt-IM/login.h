#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mainwindow.h"
#include "ipselect.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:

    void on_login_clicked();

private:
    Ui::Login *ui;
    MainWindow *mw;
    QString ip;

};

#endif // LOGIN_H
