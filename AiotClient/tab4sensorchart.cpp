#include "tab4sensorchart.h"
#include "ui_tab4sensorchart.h"

Tab4SensorChart::Tab4SensorChart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab4SensorChart)
{
    ui->setupUi(this);
    //connect(ui->pushButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    illuLine = new QLineSeries(this);
    illuLine->setName("조도");

    tempLine = new QLineSeries(this);
    tempLine->setName("온도");

    humidLine = new QLineSeries(this);
    humidLine->setName("습도");

    QPen pen;
    pen.setWidth(2);
    pen.setBrush(Qt::red);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    illuLine->setPen(pen);

    QPen tpen;
    tpen.setWidth(2);
    tpen.setBrush(Qt::green);
    tpen.setCapStyle(Qt::FlatCap);
    tpen.setJoinStyle(Qt::MiterJoin);
    tempLine->setPen(tpen);

    QPen hpen;
    hpen.setWidth(2);
    hpen.setBrush(Qt::blue);
    hpen.setCapStyle(Qt::FlatCap);
    hpen.setJoinStyle(Qt::MiterJoin);
    humidLine->setPen(hpen);

    pQChart = new QChart();
    pQChart->addSeries(illuLine);
    pQChart->addSeries(tempLine);
    pQChart->addSeries(humidLine);
    pQChart->createDefaultAxes();
    pQChart->axes(Qt::Vertical).constFirst()->setRange(0,100);

    pQChartView = new QChartView(pQChart);
    pQChartView->setRenderHint(QPainter::Antialiasing);

    ui->pChartViewLayout->layout()->addWidget(pQChartView);
    pQDateTimeAxisX = new QDateTimeAxis; //가로축
    pQDateTimeAxisX->setFormat("hh:mm:ss"); //,분 ,초2자리
    //pQDateTimeAxisX->setFormat("MM-dd hh:mm:ss"); //날짜, 시,분 ,초2자리

    updateLastDateTime(0);
    pQChartView->chart()->setAxisX(pQDateTimeAxisX, illuLine);
    pQChartView->chart()->setAxisX(pQDateTimeAxisX, tempLine);
    pQChartView->chart()->setAxisX(pQDateTimeAxisX, humidLine);
}
void Tab4SensorChart::updateLastDateTime(bool bFlag)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    firstDateTime.setDate(date);
    firstDateTime.setTime(time);

    lastDateTime.setDate(date);
    QTime tempTime = time.addSecs(10*60); //10분
    lastDateTime.setTime(tempTime);

    pQDateTimeAxisX->setRange(firstDateTime,lastDateTime);
}
Tab4SensorChart::~Tab4SensorChart()
{
    delete ui;
}

void Tab4SensorChart::tab4RecvDataSlot(QString recvData)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QDateTime xValue;
    xValue.setDate(date);
    xValue.setTime(time);
    if(xValue > lastDateTime)
    {
        lastDateTime = lastDateTime.addSecs(10*60);
        pQDateTimeAxisX->setRange(firstDateTime,lastDateTime);
    }

    QStringList strList = recvData.split("@"); //recvData : [SENSORID]SENSOR@조도@온도@습도
    QString illu = strList[3]; //조도
    QString temp = strList[4]; //온도
    QString humid = strList[5]; //습도

    illuLine->append(xValue.toMSecsSinceEpoch(), illu.toInt()); //int로 변경
    tempLine->append(xValue.toMSecsSinceEpoch(), temp.toDouble());
    humidLine->append(xValue.toMSecsSinceEpoch(), humid.toDouble());
}

void Tab4SensorChart::on_pPBClearChart_clicked()
{
    illuLine->clear();
    tempLine->clear();
    humidLine->clear();
    updateLastDateTime(0);
}

