#ifndef REGIST_H
#define REGIST_H

#include <QDialog>

namespace Ui {
class Regist;
}

class Regist : public QDialog
{
    Q_OBJECT

public:
    explicit Regist(QWidget *parent = nullptr);
    ~Regist();

private slots:
    void on_in_btn_clicked();

private:
    Ui::Regist *ui;
};

#endif // REGIST_H
