#ifndef TAB2SOCKETCLIENT_H
#define TAB2SOCKETCLIENT_H

#include <QWidget>
#include <QDebug>
#include "socketclient.h"
#include <QTime>
#include <QLineEdit>
#include "keyboard.h"

namespace Ui {
class Tab2SocketClient;
}

class Tab2SocketClient : public QWidget
{
    Q_OBJECT

public:
    explicit Tab2SocketClient(QWidget *parent = nullptr);
    ~Tab2SocketClient();
    SocketClient * getpSocketClient();

private slots:
    void on_pPBserverConnect_toggled(bool checked);
    void updateRecvDataSlot(QString);
    void socketSendToLinux(int);

    void on_pPBrecvDataClear_clicked();
    void on_pPBSend_clicked();

    //void socketLamp(bool clicked);
    //void socketPlug(bool clicked);
    void on_pLErecvId_selectionChanged();

    void on_pLEsendData_selectionChanged();

signals:
    void ledWriteSig(int);
    void tab3RecvDataSig(QString);
    void tab4RecvDataSig(QString);
    void tab5RecvDataSig(QString);
private:
    Ui::Tab2SocketClient *ui;
    SocketClient *pSocketClient;
    Keyboard *pKeyBoard;
};

#endif // TAB2SOCKETCLIENT_H
