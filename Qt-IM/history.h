#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>

#include "dbmanager.h"

namespace Ui {
class History;
}

class History : public QDialog
{
    Q_OBJECT

public:
    explicit History(int id, QString lname, QString lip, QString tname, QString tip, QWidget *parent = nullptr);
    explicit History(int id, QWidget *parent = nullptr);
    ~History();

private slots:
    void on_search_clicked();

private:
    Ui::History *ui;

    int id;
    QString localName;
    QString localIp;
    QString targetName;
    QString targetIp;
    bool isGroup;
};

#endif // HISTORY_H
