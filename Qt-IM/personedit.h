#ifndef PERSONEDIT_H
#define PERSONEDIT_H

#include <QDialog>
#include<QMessageBox>
#include "dbmanager.h"
#include <networktool.h>

namespace Ui {
class PersonEdit;
}

class PersonEdit : public QDialog
{
    Q_OBJECT

public:
    explicit PersonEdit(QWidget *parent = nullptr);
    Ui::PersonEdit *ui;
    void init(int id, bool isGroup);
    ~PersonEdit();

private:
    QString m_nicknameBefore;
    QString m_ipBefore;
    int m_id;
    bool testPersonIPFormat(QString ip);
};

#endif // PERSONEDIT_H
