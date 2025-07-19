#ifndef LEDKEYDEV_H
#define LEDKEYDEV_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QSocketNotifier>
#include <QDebug>

class LedKeyDev : public QWidget
{
    Q_OBJECT
private:
    QFile *pQFile; //디바이스 파일에 접근하기 위한 객체
    QSocketNotifier *pQSocketNotifier; //디바이스 파일의 읽기 이벤트를 감지하는 객체

    QString DEVFILENAME = "/dev/ledkey";

public:
    explicit LedKeyDev(QWidget *parent = nullptr);

signals:
    void updateKeyDataSig(int); //새로울 키 데이터가 있을 때 발생
private slots:
    void readKeyDataSlot(int); //키 데이터를 읽고 updateKeyDataSig 시그널 발생
public slots:
    void writeLedDataSlot(int); //led에 데이터 씀
};

#endif // LEDKEYDEV_H
