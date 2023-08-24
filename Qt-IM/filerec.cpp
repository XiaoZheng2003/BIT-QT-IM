#include "filerec.h"
#include "ui_filerec.h"
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>
#include <QFileDialog>


Filerec::Filerec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Filerec)
{
    ui->setupUi(this);
}

Filerec::~Filerec()
{
    delete ui;
}

void Filerec::on_button_normal_clicked()
{

    QString filename = QFileDialog::getExistingDirectory();
    QDir *dir=new QDir(filename);
    QStringList filter;
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    for(int i = 0;i<fileInfo->count(); i++)
    {
        qDebug()<<fileInfo->at(i).filePath();
        qDebug()<<fileInfo->at(i).fileName();
    }
    QString  sub="temp";
    //dir.mkdir(sub);
     ui->pathname->setText(filename+"/"+sub+"/");
}

