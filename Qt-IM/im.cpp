#include "im.h"
#include "ui_im.h"

IM::IM(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IM)
{
    ui->setupUi(this);
}

IM::~IM()
{
    delete ui;
}

