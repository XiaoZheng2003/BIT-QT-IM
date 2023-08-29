#ifndef IPSELECT_H
#define IPSELECT_H

#include <QDialog>

#include "networktool.h"

namespace Ui {
class IpSelect;
}

class IpSelect : public QDialog
{
    Q_OBJECT

public:
    explicit IpSelect(QWidget *parent = nullptr);
    ~IpSelect();

signals:
    void selectedIp(QString ip);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::IpSelect *ui;
};

#endif // IPSELECT_H
