#ifndef FILEREC_H
#define FILEREC_H

#include <QWidget>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QElapsedTimer>

class FileReceiver:public QObject
{
    Q_OBJECT
public:
    FileReceiver(QHostAddress address,qint16 tcpPort);
    QFile *localFile;

public slots:
    void startConnect(QString fileName);
    void end();
signals:
    void sendReceivedBytes(qint64 bytes,float timeUsed);
    void sendTotalBytes(qint64 bytes);
    void endConnect();
private slots:
    void readMessage();
    void displayError(QAbstractSocket::SocketError socketError);
private:
    qint16 tcpPort;
    qint64 fileNameSize;
    qint64 totalBytes;
    qint64 receivedBytes;
    QString fileName;
    QTcpSocket *tcpClient;
    QHostAddress hostAddress;
    QByteArray inBlock;
    QElapsedTimer time;
};
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

public slots:
    void endConnect();
    void setTotalBytes(qint64 totalBytes);
    void getReceivedBytes(qint64 totalBytes,float timeUsed);

private slots:
    //void on_button_normal_clicked();
    void newConnect();

    void on_cancelButton_clicked();
    void on_finishButton_clicked();

signals:
    void startConnect(QString fileName);
    void quit();
private:
    Ui::Filerec *ui;
    FileReceiver *fileReceiver;
    qint16  tcpPort ;
    QHostAddress hostAddress;
    qint64 totalBytes ;
    qint64 bytesReceived;
    QString fileName;
    QByteArray inBlock;
};
#endif // FILEREC_H
