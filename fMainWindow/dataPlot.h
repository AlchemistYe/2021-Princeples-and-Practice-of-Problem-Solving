#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <QWidget>
#include <QMainWindow>
#include "dataBase.h"
#include <QStringList>
#include <QtCharts>
#include <QChartView>
namespace Ui {
class dataPlot;
}

class dataPlot : public QMainWindow
{
    Q_OBJECT

public:
    explicit dataPlot(QMainWindow *parent = nullptr);
    ~dataPlot();
    void copyPOI(QVector<POI>);
    void topTen1();
    void topTen2();
    void getNum1();
    void getNum2();
    bool isEmpty();

private:
    Ui::dataPlot *ui;
    QVector<POI> plotOut;

};

#endif // DATAPLOT_H
