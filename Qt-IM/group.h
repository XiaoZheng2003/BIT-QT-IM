#ifndef GROUP_H
#define GROUP_H

#include <QDialog>
#include "heartbeatbroadcaster.h"

namespace Ui {
class Group;
}

class Group : public QDialog
{
    Q_OBJECT

public:
    explicit Group(HeartbeatBroadcaster* reci, QWidget *parent = nullptr);
    ~Group();

private:
    Ui::Group *ui;
    HeartbeatBroadcaster * receiver;

private slots:
    void refresh();
};

#endif // GROUP_H
