#ifndef FILEREC_H
#define FILEREC_H

#include <QWidget>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QElapsedTimer>

namespace Ui {
class Filerec;
}

class Filerec : public QWidget
{
    Q_OBJECT

public:
    explicit Filerec(QWidget *parent = nullptr);
    ~Filerec();
    void setFileName(QString fileName);
    void setHostAddress(QHostAddress address);
    void closeEvent(QCloseEvent *);
    QFile *localFile;

private slots:
    //void on_button_normal_clicked();
    void displayError(QAbstractSocket::SocketError);
    void newConnect();

    void readMessage();
    void on_cancelButton_clicked();
    void on_finishButton_clicked();

private:
    Ui::Filerec *ui;
    QTcpSocket *tcpClient;
    qint16  tcpPort ;
    QHostAddress hostAddress;
    qint64 TotalBytes ;
    qint64 bytesReceived;
    qint64 fileNameSize ;
    qint64 blockSize;
    QString fileName;
    QElapsedTimer time;
    QByteArray inBlock;
};

#endif // FILEREC_H
