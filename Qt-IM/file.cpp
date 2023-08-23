#include "file.h"
#include "ui_file.h"

file::file(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file)
{
    ui->setupUi(this);
}

file::~file()
{
    delete ui;
}
