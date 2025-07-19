#include "tab1devcontrol.h"
#include "ui_tab1devcontrol.h"

Tab1DevControl::Tab1DevControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab1DevControl)
{
    ui->setupUi(this);
    int keyCount = ui->gridLayout->rowCount() * ui->gridLayout->columnCount();
    pLedKeyDev = new LedKeyDev(this);
    pQTimer = new QTimer(this);
    pQButtonGroup = new QButtonGroup(this);
    for(int i = 0; i<ui->gridLayout->rowCount();i++)
    {
        for(int j = 0; j<ui->gridLayout->columnCount();j++)
        {
            pQCheckBox[--keyCount] = dynamic_cast<QCheckBox*>(ui->gridLayout->itemAtPosition(i,j)->widget());
        }
    }
    pQButtonGroup->setExclusive(false);
    keyCount = ui->gridLayout->rowCount() * ui->gridLayout->columnCount(); //다시 계산
    for(int i = 0; i < keyCount; i++)
        pQButtonGroup->addButton(pQCheckBox[i],i+1);
    connect(pQButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateCheckBoxMouseSlot(int))); //QT5에서 사용-> 라즈베리파이에서 가능
    //connect(pQButtonGroup, SIGNAL(idClicked(int)), this, SLOT(updateCheckBoxMouseSlot(int))); //QT6에서 사용
    connect(pQTimer, SIGNAL(timeout()), this, SLOT(updateDialValueSlot()));
    connect(ui->pDialLed, SIGNAL(valueChanged(int)), pLedKeyDev, SLOT(writeLedDataSlot(int)));
    //connect(ui->pDialLed, SIGNAL(valueChanged(int)),this,SLOT(updateProgressBarLedSlot(int))); //간접 연결 방식
    connect(ui->pDialLed, SIGNAL(valueChanged(int)),ui->pProgressBarLed,SLOT(setValue(int))); //직접 연결 방식
    connect(ui->pPBquit, SIGNAL(clicked()), this, SLOT(widgetquit()));
    connect(pLedKeyDev, SIGNAL(updateKeyDataSig(int)),this,SLOT(updateCheckBoxKeySlot(int)));
}
void Tab1DevControl::updateProgressBarLedSlot(int value)
{
    ui->pProgressBarLed->setValue(value);
}
Tab1DevControl::~Tab1DevControl()
{
    delete ui;
}

void Tab1DevControl::on_pPBtimerStart_clicked(bool checked)
{
    if(checked)
    {
        QString strValue = ui->pCBtimerValue->currentText();
        pQTimer->start(strValue.toInt());
        ui->pPBtimerStart->setText("TimerStop");
    }
    else
    {
        pQTimer->stop();
        ui->pPBtimerStart->setText("TimerStart");
    }
}

void Tab1DevControl::updateDialValueSlot()
{
    int dialValue = ui->pDialLed->value();
    dialValue++; //값 증가
    if(dialValue > ui->pDialLed->maximum()) //255보다 크면
        dialValue = 0;
    ui->pDialLed->setValue(dialValue);

}

void Tab1DevControl::on_pCBtimerValue_currentTextChanged(const QString &arg1)
{
    if(pQTimer->isActive())
    {
        pQTimer->stop();
        pQTimer->start(arg1.toInt());
    }
}

void Tab1DevControl::widgetquit()
{
    QApplication::quit();
}

void Tab1DevControl::updateCheckBoxKeySlot(int keyNo)
{
    //QCheckBox *pQcheckBox[8] = {ui->pCBkey1,ui->pCBkey2, ui->pCBkey3,ui->pCBkey4,ui->pCBkey5,ui->pCBkey6,ui->pCBkey7,ui->pCBkey8};
    //static unsigned char lcdData = 0;
    lcdData = lcdData ^ (0x01 << (keyNo - 1));
    ui->pLcdNumberKey->display(lcdData);
    pLedKeyDev->writeLedDataSlot(lcdData);
    for(int i = 0; i<8; i++)
    {
        if(keyNo == i+1)
        {
            if(pQCheckBox[i]->isChecked())
            {
                pQCheckBox[i]->setChecked(false); //버튼 상태 초기화
            }
            else
                pQCheckBox[i]->setChecked(true);
        }
        ui->pPBtimerStart->setChecked(false);   // 버튼 상태 초기화
        on_pPBtimerStart_clicked(false); //pQTimer->stop(); , ui->pPBtimerStart->setText("TimerStart")
        ui->pDialLed->setValue(lcdData);
    }
}

void Tab1DevControl::updateCheckBoxMouseSlot(int keyNo) //마우스로 체크박스 체크할 때 함수
{
    qDebug() << keyNo;
    lcdData = lcdData ^ (0x01 << (keyNo - 1));
    ui->pLcdNumberKey->display(lcdData);
    //pLedKeyDev->writeLedDataSlot(lcdData);
    //if(ui->pPBtimerStart->isChecked())
    {
        ui->pPBtimerStart->setChecked(false);   // 버튼 상태 초기화
        on_pPBtimerStart_clicked(false); //pQTimer->stop(); , ui->pPBtimerStart->setText("TimerStart")
        ui->pDialLed->setValue(lcdData);
    }
}

QDial* Tab1DevControl::getpDial()
{
    return ui->pDialLed;
}

LedKeyDev* Tab1DevControl::getpLedKeyDev()
{
    return pLedKeyDev;
}
