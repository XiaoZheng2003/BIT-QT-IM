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
    explicit Emoji(int px, int py, QWidget *parent = nullptr);
    ~Emoji();

private slots:
    void on_emojiTable_cellClicked(int row, int column);

private:
    Ui::Emoji *ui;

signals:
    void addEmoji(QString emoji);
};

#endif // EMOJI_H
