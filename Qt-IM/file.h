#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QString>
namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();

private slots:
    void on_Button_choose_clicked();

private:
    Ui::File *ui;
    QString fname;

};

#endif // FILE_H
