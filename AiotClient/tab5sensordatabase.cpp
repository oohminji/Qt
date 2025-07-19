#include "tab5sensordatabase.h"
#include "ui_tab5sensordatabase.h"

Tab5SensorDatabase::Tab5SensorDatabase(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab5SensorDatabase)
{
    ui->setupUi(this);
    qSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    qSqlDatabase.setDatabaseName("aiot.db");
    if(qSqlDatabase.open())
        qDebug() << "Success DB Connection";
    else
        qDebug() << "Fail DB Connection";
    QString strQuery = "create table sensor_tb("
                        "name varchar(10),"
                        "date DATETIME primary key,"
                        "illu varchar(10),"
                        "temp varchar(10),"
                        "humid varchar(10))";
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(strQuery))
        qDebug() << "Create Table";

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
void Tab5SensorDatabase::updateLastDateTime(bool bFlag)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    firstDateTime.setDate(date);
    firstDateTime.setTime(time);

    lastDateTime.setDate(date);
    QTime tempTime = time.addSecs(60); //10분
    lastDateTime.setTime(tempTime);

    pQDateTimeAxisX->setRange(firstDateTime,lastDateTime);
}

Tab5SensorDatabase::~Tab5SensorDatabase()
{
    delete ui;
}

void Tab5SensorDatabase::tab5RecvDataSlot(QString recvData)
{
    // QDate date = QDate::currentDate();
    // QTime time = QTime::currentTime();
    // QDateTime xValue;
    // xValue.setDate(date);
    // xValue.setTime(time);

    QDateTime dateTime = QDateTime::currentDateTime();
    QStringList strList = recvData.split("@"); //recvData : [SENSORID]SENSOR@조도@온도@습도
    QString name = strList[1];
    QString illu = strList[3]; //조도
    QString temp = strList[4]; //온도
    QString humid = strList[5]; //습도

    //illuLine->append(dateTime.toMSecsSinceEpoch(), illu.toInt()); //int로 변경
    // tempLine->append(xValue.toMSecsSinceEpoch(), temp.toDouble());
    // humidLine->append(xValue.toMSecsSinceEpoch(), humid.toDouble());

    QString strQuery = "insert into sensor_tb(name, date, illu, temp, humid) values('" + name + "', '" + dateTime.toString("yyyy/MM/dd hh:mm:ss") + "' , '" + illu + "', '" + temp + "', '" + humid + "')";
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(strQuery))
        qDebug() << "Insert Query Ok";
    else
        qDebug() << "Insert Query Failed: " << sqlQuery.lastError().text();

    if(dateTime > lastDateTime) //원하는 시간 만큼 추가
    {
        lastDateTime = lastDateTime.addSecs(10*60);
        pQDateTimeAxisX->setRange(firstDateTime,lastDateTime);
    }
}

void Tab5SensorDatabase::on_pPBClear_clicked() //clear 버튼 누를 시 호출하는 함수
{
    illuLine->clear(); //그래프 지우기
    tempLine->clear();
    humidLine->clear();
    updateLastDateTime(0);
}
void Tab5SensorDatabase::on_pPBsearchDB_clicked() //조회 버튼 누를 시 호출하는 함수
{
    QDateTime fromDateTime = ui->pDateTimeEditFrom->dateTime();
    QString strFromDataTime = fromDateTime.toString("yyyy/MM/dd hh:mm:ss");
    QDateTime toDateTime = ui->pDateTimeEditTo->dateTime();
    QString strToDateTime = toDateTime.toString("yyyy/MM/dd hh:mm:ss");

    //ui->pTBsensor->clearContents();
    //on_pPBClear_clicked();

    //SQL쿼리 문자열 생성
    QString strQuery = "select * from sensor_tb where date > '" + strFromDataTime + "' <= date and date < '" + strToDateTime + "'";
    QSqlQuery sqlQuery; //쿼리를 실행할 객체 생성

    // 쿼리 문장을 실행, 성공 시 1 return
    if(sqlQuery.exec(strQuery))
    {
        qDebug() << "Select Query OK..";
        int rowCount = 0;
        while(sqlQuery.next())
        {
            rowCount++;
        }
            if(pQTableWidgetItemId != nullptr)
            {
                delete [] pQTableWidgetItemId;
                delete [] pQTableWidgetItemDate;
                delete [] pQTableWidgetItemIllu;
                delete [] pQTableWidgetItemTemp;
                delete [] pQTableWidgetItemHumid;
            }

            pQTableWidgetItemId = new QTableWidgetItem[rowCount];
            pQTableWidgetItemDate = new QTableWidgetItem[rowCount];
            pQTableWidgetItemIllu = new QTableWidgetItem[rowCount];
            pQTableWidgetItemTemp = new QTableWidgetItem[rowCount];
            pQTableWidgetItemHumid = new QTableWidgetItem[rowCount];

            rowCount=0;
            sqlQuery.first();
            while(sqlQuery.next())
            {

            ui->pTBsensor->setRowCount(rowCount+1);
            // QTableWidgetItem * pQTableWidgetItemId = new QTableWidgetItem();
            // QTableWidgetItem * pQTableWidgetItemDate = new QTableWidgetItem();
            // QTableWidgetItem * pQTableWidgetItemIllu = new QTableWidgetItem();

            // pQTableWidgetItemId->setText(sqlQuery.value("name").toString());
            // pQTableWidgetItemDate->setText(sqlQuery.value("date").toString());
            // pQTableWidgetItemIllu->setText(sqlQuery.value("illu").toString());

            // ui->pTBsensor->setItem(rowCount-1,0,pQTableWidgetItemId);
            // ui->pTBsensor->setItem(rowCount-1,1,pQTableWidgetItemDate);
            // ui->pTBsensor->setItem(rowCount-1,2,pQTableWidgetItemIllu);


            (pQTableWidgetItemId+rowCount)->setText(sqlQuery.value("name").toString());
            (pQTableWidgetItemDate+rowCount)->setText(sqlQuery.value("date").toString());
            (pQTableWidgetItemIllu+rowCount)->setText(sqlQuery.value("illu").toString());
            (pQTableWidgetItemTemp+rowCount)->setText(sqlQuery.value("temp").toString());
            (pQTableWidgetItemHumid+rowCount)->setText(sqlQuery.value("humid").toString());

            ui->pTBsensor->setItem(rowCount,0, (pQTableWidgetItemId+rowCount));
            ui->pTBsensor->setItem(rowCount,1, (pQTableWidgetItemDate+rowCount));
            ui->pTBsensor->setItem(rowCount,2, (pQTableWidgetItemIllu+rowCount));
            ui->pTBsensor->setItem(rowCount,3, (pQTableWidgetItemTemp+rowCount));
            ui->pTBsensor->setItem(rowCount,4, (pQTableWidgetItemHumid+rowCount));

            //QDateTime xValue = QDateTime::fromString((pQTableWidgetItemDate+rowCount)->text(), "yyyy/MM/dd hh:mm:ss");
             QDateTime dateTime = QDateTime::fromString(sqlQuery.value("date").toString(), "yyyy/MM/dd hh:mm:ss"); //조회했을 때 데이터를 그래프로
            illuLine->append(dateTime.toMSecsSinceEpoch(),(pQTableWidgetItemIllu+rowCount)->text().toInt());
            tempLine->append(dateTime.toMSecsSinceEpoch(),(pQTableWidgetItemTemp+rowCount)->text().toDouble());
            humidLine->append(dateTime.toMSecsSinceEpoch(),(pQTableWidgetItemHumid+rowCount)->text().toDouble());
            rowCount++;


           // QDateTime dateTime = QDateTime::fromString(sqlQuery.value("date").toString(), "yyyy/MM/dd hh:mm:ss"); //조회했을 때 데이터를 그래프로
            // if(rowCount == 1)
            // {
            //     firstDateTime = dateTime;
            //     pQDateTimeAxisX->setRange(firstDateTime,lastDateTime);
            // }
            // illuLine->append(dateTime.toMSecsSinceEpoch(), sqlQuery.value("illu").toInt());
        }
        ui->pTBsensor->resizeColumnToContents(0); //표 사이즈 글자 사이즈에 맞추기
        ui->pTBsensor->resizeColumnToContents(1);
        ui->pTBsensor->resizeColumnToContents(2);
        ui->pTBsensor->resizeColumnToContents(3);
        ui->pTBsensor->resizeColumnToContents(4);
    }
}

void Tab5SensorDatabase::on_pPBdeleteDB_clicked() //삭제 버튼 누를 시 호출하는 함수
{
    QDateTime fromDateTime = ui->pDateTimeEditFrom->dateTime();
    QString strFromDateTime = fromDateTime.toString("yyyy/MM/dd hh:mm:ss");
    QDateTime toDateTime = ui->pDateTimeEditTo->dateTime();
    QString strToDateTime = toDateTime.toString("yyyy/MM/dd hh:mm:ss");

    QString strQuery = "delete from sensor_tb where '" + strFromDateTime + "' <= date AND date <= '" + strToDateTime + "'";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(strQuery))
        qDebug() << "Delete Query Ok";

    //ui->pTBsensor->clearContents();
    illuLine->clear(); //그래프 지우기
    tempLine->clear();
    humidLine->clear();
}
