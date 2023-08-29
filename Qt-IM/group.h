#ifndef GROUP_H
#define GROUP_H

#include <QDialog>

namespace Ui {
class Group;
}

class Group : public QDialog
{
    Q_OBJECT

public:
    explicit Group(QWidget *parent = nullptr);
    ~Group();

private:
    Ui::Group *ui;
};

#endif // GROUP_H
