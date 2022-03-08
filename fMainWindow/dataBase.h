#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QVector>
#include <QApplication>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlError>

class POI{
public:
    int user;
    int location;
    qlonglong time;
    double latitude;
    double longitude;
};

class dataBase : public QWidget
{
    Q_OBJECT

public:
    explicit dataBase(QWidget *parent = nullptr);
    void createTable();
    void getPOI(QString);
    bool isEmpty();
    qlonglong toLongInt(QString);
    void userClicked(QString,QString);
    void locationClicked(QString,QString);
    void timeClicked(QString,QString);
    void latitudeClicked(QString,QString);
    void longitudeClicked(QString,QString);
    void filter();
    QVector<POI>checkIn;
    QSqlDatabase db;

signals:
    void sendLength(int);
    void sendRow(int);
    void sendPOI(QVector<POI>);


private:
    QString fileName;   
    POI beginPOI;
    POI endPOI;


};

#endif // DATABASE_H
