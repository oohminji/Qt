#ifndef TAB3CONTROLPANNEL_H
#define TAB3CONTROLPANNEL_H

#include <QWidget>
#include <QDebug>
#include <QPalette>
#include "socketclient.h"

namespace Ui {
class Tab3ControlPannel;
}

class Tab3ControlPannel : public QWidget
{
    Q_OBJECT

public:
    explicit Tab3ControlPannel(QWidget *parent = nullptr);
    ~Tab3ControlPannel();

private slots:
    // void on_pPBlamp_toggled(bool checked);

    // void on_pPBplug_toggled(bool checked);

    void tab3RecvDataSlot(QString);
    void on_pPBlamp_clicked(bool checked);

    void on_pPBplug_clicked(bool checked);

signals:
    //void LampSig(bool);
    //void PlugSig(bool);
    void socketSendDataSig(QString);
private:
    Ui::Tab3ControlPannel *ui;
    SocketClient *pControlPannel;
    QPalette palletteOn;
    QPalette palletteOff;


};

#endif // TAB3CONTROLPANNEL_H
