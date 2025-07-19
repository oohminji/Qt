#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QWidget>
#include <QTcpSocket> //네트워크 모듈 추가
#include <QHostAddress>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

#define BLOCK_SIZE 1024
class SocketClient : public QWidget
{
    Q_OBJECT
    QTcpSocket *pQTcpSocket;
    //QString SERVERIP = "192.168.0.93";
    QString SERVERIP = "192.168.0.4";
    int SERVERPORT = 5000;
    //QString LOGID = "MJ_QT";
    QString LOGID = "15";
    QString LOGPW = "PASSWD";
public:
    explicit SocketClient(QWidget *parent = nullptr);
    ~SocketClient();

signals:
    void socketRecvDataSig(QString strRecvData);

private slots:
    void socketReadDataSlot();
    void socketErrorSlot();
    void socketConnectServerSlot();

public slots:
    void connectToServerSlot(bool &);
    void socketClosedServerSlot();
    void socketWriteDataSlot(QString);

signals:
};

#endif // SOCKETCLIENT_H
