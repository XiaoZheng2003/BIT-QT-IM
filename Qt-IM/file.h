#ifndef FILE_H
#define FILE_H

#include <QWidget>

namespace Ui {
class file;
}

class file : public QWidget
{
    Q_OBJECT

public:
    explicit file(QWidget *parent = nullptr);
    ~file();

private:
    Ui::file *ui;
};

#endif // FILE_H
