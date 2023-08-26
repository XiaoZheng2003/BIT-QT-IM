#include "emoji.h"
#include "ui_emoji.h"

Emoji::Emoji(int px, int py, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Emoji)
{
    ui->setupUi(this);

    ui->emojiTable->horizontalHeader()->setHidden(true);
    ui->emojiTable->verticalHeader()->setHidden(true);//删除表头表尾
    for (int row = 0; row < 5; row++) {
        ui->emojiTable->setRowHeight(row,50);
    }
    for (int col = 0; col < 5; col++) {
        ui->emojiTable->setColumnWidth(col,50);
    }
    ui->emojiTable->setFont(QFont("Arial", 25));
    ui->emojiTable->setStyleSheet("QTableWidget::item {text-align: center;}");
    ui->emojiTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->move(px,py-this->height());
}

Emoji::~Emoji()
{
    delete ui;
}

void Emoji::on_emojiTable_cellClicked(int row, int column)
{
    emit addEmoji(ui->emojiTable->item(row,column)->text());
    this->close();
}
