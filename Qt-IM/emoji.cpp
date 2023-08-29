#include "emoji.h"
#include "ui_emoji.h"

Emoji::Emoji(int px, int py, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Emoji)
{
    ui->setupUi(this);

    ui->emojiTable->horizontalHeader()->setHidden(true);
    ui->emojiTable->verticalHeader()->setHidden(true);//删除表头表尾

    ui->emojiTable->setFont(QFont("Arial", 25));
    ui->emojiTable->setStyleSheet("QTableWidget::item {text-align: center;}");
    ui->emojiTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑


    ui->emojiTable->resizeRowsToContents();
    ui->emojiTable->resizeColumnsToContents();//单元格自适应大小



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
