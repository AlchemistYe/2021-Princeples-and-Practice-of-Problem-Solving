#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QDebug>
#include <QAction>
#include <QtSql/QSqlDatabase>
#include "dataPlot.h"
#include "comparePlot.h"
#include "dataBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendFile(QString);
    void sendUser(QString,QString);
    void sendLocation(QString,QString);
    void sendTime(QString,QString);
    void sendLatitude(QString,QString);
    void sendLongitude(QString,QString);
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
