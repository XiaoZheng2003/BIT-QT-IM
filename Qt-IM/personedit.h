#ifndef PERSONEDIT_H
#define PERSONEDIT_H

#include <QDialog>
#include "dbmanager.h"

namespace Ui {
class PersonEdit;
}

class PersonEdit : public QDialog
{
    Q_OBJECT

public:
    explicit PersonEdit(QWidget *parent = nullptr);
    ~PersonEdit();

private slots:
    void init(QString nickname, QString ip);
    void commitChange();

private:
    Ui::PersonEdit *ui;
    QString m_nicknameBefore;
    QString m_ipBefore;
};

#endif // PERSONEDIT_H
