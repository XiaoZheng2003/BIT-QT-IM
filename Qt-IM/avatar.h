#ifndef AVATAR_H
#define AVATAR_H

#include <QDialog>

namespace Ui {
class Avatar;
}

class Avatar : public QDialog
{
    Q_OBJECT

public:
    explicit Avatar(QWidget *parent = nullptr);
    ~Avatar();

    void init(int aid);

private slots:
    void on_avatar0_clicked();

    void on_avatar1_clicked();

    void on_avatar2_clicked();

    void on_avatar3_clicked();

    void on_avatar4_clicked();

    void on_avatar5_clicked();

    void on_buttonBox_accepted();

signals:
    void changeAvatarId(int id);

private:
    Ui::Avatar *ui;

    int avatarId;
};

#endif // AVATAR_H
