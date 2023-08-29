#include "ipselect.h"
#include "ui_ipselect.h"

IpSelect::IpSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpSelect)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);
    QStringList ips=NetworkTool::GetLocalIP().split('|');
    ui->tip->setText(ui->tip->text().arg(ips.count()));
    ui->ipSelect->addItems(ips);
}

IpSelect::~IpSelect()
{
    delete ui;
}

void IpSelect::on_buttonBox_accepted()
{
    int t=ui->ipSelect->currentIndex();
    emit selectedIp(NetworkTool::GetLocalIP().split('|')[t]);
    this->close();
}
