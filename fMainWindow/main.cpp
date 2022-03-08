#include <dataPlot.h>
#include "mainwindow.h"
#include "dataBase.h"
#include "comparePlot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
