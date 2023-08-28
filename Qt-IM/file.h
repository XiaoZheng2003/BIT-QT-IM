#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QElapsedTimer>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    void init();
    void refused();
    QTcpSocket* get_Client_Connection();

protected:
    void closeEvent(QCloseEvent *);

signals:
    void sendFileName(QString);

private slots:
    void on_Button_choose_clicked();

    void on_Button_send_clicked();

    void on_Button_cancle_clicked();

    void sendFile();

    void updateClientProgress(qint64 );

private:
    Ui::File *ui;
    QString fname;
    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    qint16 tcpPort;
    QFile *localFile ;
    qint64 payloadSize ;
    qint64 TotalBytes ;
    qint64 bytesWritten ;
    qint64 bytestoWrite;
    QString theFileName;
    QString fileName;
    QElapsedTimer time;
    QByteArray outBlock;

};

#endif // FILE_H
