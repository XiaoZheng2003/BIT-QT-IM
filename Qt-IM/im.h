#ifndef IM_H
#define IM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class IM; }
QT_END_NAMESPACE

class IM : public QMainWindow
{
    Q_OBJECT

public:
    IM(QWidget *parent = nullptr);
    ~IM();

private:
    Ui::IM *ui;
};
#endif // IM_H
