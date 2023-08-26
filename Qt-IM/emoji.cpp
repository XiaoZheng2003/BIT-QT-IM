#include "emoji.h"
#include "ui_emoji.h"

Emoji::Emoji(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Emoji)
{
    ui->setupUi(this);


    ui->tableWidget->horizontalHeader()->setHidden(true);
    ui->tableWidget->verticalHeader()->setHidden(true);//删除表头表尾
    for (int row = 0; row < 5; row++) {
        ui->tableWidget->setRowHeight(row,50);
    }
    for (int col = 0; col < 5; col++) {
        ui->tableWidget->setColumnWidth(col,50);
    }
    ui->tableWidget->setFont(QFont("Arial", 25));
    ui->tableWidget->setStyleSheet("QTableWidget::item {text-align: center;}");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑



}

Emoji::~Emoji()
{
    delete ui;
}


