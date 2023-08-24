#ifndef FILEREC_H
#define FILEREC_H

#include <QWidget>

namespace Ui {
class Filerec;
}

class Filerec : public QWidget
{
    Q_OBJECT

public:
    explicit Filerec(QWidget *parent = nullptr);
    ~Filerec();

private slots:
    void on_button_normal_clicked();

private:
    Ui::Filerec *ui;
};

#endif // FILEREC_H
