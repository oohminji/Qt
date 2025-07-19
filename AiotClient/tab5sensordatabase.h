#ifndef TAB5SENSORDATABASE_H
#define TAB5SENSORDATABASE_H

// #include <QWidget>
// #include <QSqlDatabase>
// #include <QSqlQuery>
// #include <QSqlError>
// #include <QDateTime>
// #include <QChartView>
// #include <QLineSeries>
// #include <QDateTimeAxis>
// #include <QDebug>

// #include <QTableWidgetItem>

#include <QWidget>
#include <QTime>
#include <QDate>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#else
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
QT_CHARTS_USE_NAMESPACE
#endif
namespace Ui {
class Tab5SensorDatabase;
}

class Tab5SensorDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit Tab5SensorDatabase(QWidget *parent = nullptr);
    ~Tab5SensorDatabase();

private:
    Ui::Tab5SensorDatabase *ui;
    QSqlDatabase qSqlDatabase;
    QLineSeries *illuLine;
    QLineSeries *tempLine;
    QLineSeries *humidLine;
    QChart* pQChart;
    QChartView* pQChartView;
    QDateTimeAxis* pQDateTimeAxisX;
    QDateTime firstDateTime;
    QDateTime lastDateTime;
    //-------------------------------------------------교수님
    QTableWidgetItem* pQTableWidgetItemId = nullptr;
    QTableWidgetItem* pQTableWidgetItemDate = nullptr;
    QTableWidgetItem* pQTableWidgetItemIllu = nullptr;
    QTableWidgetItem* pQTableWidgetItemTemp = nullptr;
    QTableWidgetItem* pQTableWidgetItemHumid = nullptr;
    //-------------------------------------------------교수님

    void updateLastDateTime(bool);
private slots:
    void tab5RecvDataSlot(QString);
    void on_pPBClear_clicked();
    void on_pPBsearchDB_clicked();
    void on_pPBdeleteDB_clicked();
};

#endif // TAB5SENSORDATABASE_H
