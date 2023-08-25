#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include <QMessageBox>
#include <QList>
#include "dbmanager.h"
#include "networktool.h"

namespace Ui {
class Edit;
}

class Edit : public QDialog
{
    Q_OBJECT

public:
    explicit Edit(QWidget *parent = nullptr);
    Ui::Edit *ui;
    void init(int id, bool isGroup);
    ~Edit();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    QString m_nicknameBefore;
    QString m_ipBefore;
    int m_id;
    bool m_isGroup;
};

#endif // EDIT_H
