#include "tab3controlpannel.h"
#include "ui_tab3controlpannel.h"

Tab3ControlPannel::Tab3ControlPannel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab3ControlPannel)
{
    ui->setupUi(this);
    palletteOn.setColor(ui->pPBlamp->backgroundRole(),QColor(255,0,0));
    palletteOff.setColor(ui->pPBlamp->backgroundRole(),QColor(0,255,0));
    ui->pPBlamp->setPalette(palletteOff);
    ui->pPBplug->setPalette(palletteOff);
    pControlPannel = new SocketClient(this);
}

Tab3ControlPannel::~Tab3ControlPannel()
{
    delete ui;
}
/*
void Tab3ControlPannel::on_pPBlamp_toggled(bool checked)
{
    emit LampSig(checked);

}

void Tab3ControlPannel::on_pPBplug_toggled(bool checked)
{
    emit PlugSig(checked);
}
*/
void Tab3ControlPannel::tab3RecvDataSlot(QString recvData)
{

    QStringList strList = recvData.split("@");
    if(strList[2] == "LAMPON")
    {
        ui->pPBlamp->setChecked(true);
        ui->pPBlamp->setPalette(palletteOn);
    }
    else if(strList[2] == "LAMPOFF")
    {
        ui->pPBlamp->setChecked(false);
        ui->pPBlamp->setPalette(palletteOff);
    }
    else if(strList[2] == "PLUGON")
    {
        ui->pPBplug->setChecked(true);
        ui->pPBplug->setPalette(palletteOn);
    }
    else if(strList[2] == "PLUGOFF")
    {
        ui->pPBplug->setChecked(false);
        ui->pPBplug->setPalette(palletteOff);
    }

}

void Tab3ControlPannel::on_pPBlamp_clicked(bool checked)
{
    if(checked)
    {
        // ON 상태일 때
        emit socketSendDataSig("[MJ_LIN]LAMPON");
        ui->pPBlamp->setChecked(false);
    }
    else
    {
        emit socketSendDataSig("[MJ_LIN]LAMPOFF");
        ui->pPBplug->setChecked(true);
    }
}


void Tab3ControlPannel::on_pPBplug_clicked(bool checked)
{
    if(checked)
    {
        // ON 상태일 때
        emit socketSendDataSig("[MJ_LIN]PLUGON");
        ui->pPBplug->setChecked(false);
    }
    else
    {
        emit socketSendDataSig("[MJ_LIN]PLUGOFF");
        ui->pPBplug->setChecked(true);
    }
}

