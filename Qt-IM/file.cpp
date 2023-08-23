#include "file.h"
#include "ui_file.h"
#include <QFileDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
}

File::~File()
{
    delete ui;
}

void File::on_Button_choose_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("选择一个文件"),".",tr("All Files(*.*)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,tr("打开文件"),
            tr("打开文件失败:\n%1").arg(path));
            return;
        }
        //QTextStream in(&file);
        fname=path;
        if(!fname.isEmpty())
        {
            QMessageBox::information(this,tr("信息"),tr("选择文件成功."));
            QStringList tmp=fname.split('/');
            ui->file->setText(tmp[tmp.length()-1]);
        }
        //ui->textEdit->setText(in.readAll());
        file.close();

    }else {
        QMessageBox::warning(this,tr("Path"),tr("未选择文件."));
    }
}

