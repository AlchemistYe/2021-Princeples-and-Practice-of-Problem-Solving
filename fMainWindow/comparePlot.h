#ifndef COMPAREPLOT_H
#define COMPAREPLOT_H

#include <QWidget>
#include <QMainWindow>
#include <dataBase.h>
#include <dataPlot.h>
#include <QTableWidget>
namespace Ui {
class comparePlot;
}

class comparePlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit comparePlot(QMainWindow *parent = nullptr);
    ~comparePlot();
    void copyPOI(QVector<POI>);
    void copyBase(QSqlDatabase &);
    void userFilter();
    void locationFilter();
    void userPlot();
    void locationPlot(int,int);

private:
    Ui::comparePlot *ui;
    QVector<POI> comPlot1;
    QVector<POI> comPlot2;
    QVector<qlonglong>first;
    QVector<int>DAU1;
    QVector<int>DAU2;
    QVector<int>time1;
    QVector<int>time2;
    QSqlDatabase db;
    void getDAU(qlonglong,int,int);

};

#endif // COMPAREPLOT_H
