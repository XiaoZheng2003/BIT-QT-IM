#ifndef EMOJI_H
#define EMOJI_H

#include <QDialog>

namespace Ui {
class Emoji;
}

class Emoji : public QDialog
{
    Q_OBJECT

public:
    explicit Emoji(QWidget *parent = nullptr);
    ~Emoji();

private:
    Ui::Emoji *ui;
};

#endif // EMOJI_H
