#include "ledkeydev.h"

LedKeyDev::LedKeyDev(QWidget *parent)
    : QWidget{parent}
{
    int ledKeyFd;
    pQFile = new QFile(DEVFILENAME);
    if(!pQFile->open(QFile::ReadWrite | QFile::Unbuffered)) //ReadWrite | Unbuffered 모드로 open
    {
        QMessageBox::information(this,"open","open fail : "+DEVFILENAME);
    }
    ledKeyFd = pQFile->handle(); //파일 디스크립터 얻음
    pQSocketNotifier = new QSocketNotifier(ledKeyFd, QSocketNotifier::Read,this);
    connect(pQSocketNotifier, SIGNAL(activated(int)),this,SLOT(readKeyDataSlot(int))); //읽기이벤트가 발생하면
    writeLedDataSlot(0); //초기화
}

void LedKeyDev::readKeyDataSlot(int)
{
    char keyNo = 0;
    int ret = pQFile->read(&keyNo,sizeof(keyNo)); //키 읽기
    if(ret > 0)
        emit updateKeyDataSig(int(keyNo)); //읽을 데이터가 있으면 시그널 발생
    qDebug() << int(keyNo);
}
void LedKeyDev::writeLedDataSlot(int ledValue)
{
    char ledNo = (char)ledValue;
    pQFile->write(&ledNo, sizeof(ledNo)); //led데이터 쓰기
    qDebug() << (unsigned char)(ledNo);

}
