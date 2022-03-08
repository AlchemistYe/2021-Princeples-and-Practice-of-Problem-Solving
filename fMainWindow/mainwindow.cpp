#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->proBar->hide();
    ui->filterBox->hide();
    ui->proBar->setValue(0);
    ui->proBar->setRange(0,1502356);
    dataBase *myWork=new dataBase;
    connect(myWork,&dataBase::sendRow,this,[=](int row){
        ui->proBar->setValue(row);
        ui->proBar->setFormat("%p%");
    });
    connect(this,&MainWindow::sendFile,myWork,&dataBase::getPOI);
    connect(ui->upload,&QAction::triggered,this,[=](){
        ui->proBar->show();
        QString name=QFileDialog::getOpenFileName(this,tr("Excel File"),"./",tr("Files(*.csv)"));
        if(!name.isEmpty()){
            emit sendFile(name);
            myWork->createTable();
            ui->proBar->hide();            
        }
    });
    connect(ui->actionFilter,&QAction::triggered,ui->filterBox,[=](){
        if(myWork->isEmpty())
         QMessageBox::information(this,tr(""),tr("No Data Exists"));
        else ui->filterBox->show();
    });
    connect(this,&MainWindow::sendUser,myWork,&dataBase::userClicked);
    connect(ui->userButton,&QPushButton::clicked,myWork,[=](){
        emit sendUser(ui->lineEdit1->text(),ui->lineEdit2->text());
    });
    connect(this,&MainWindow::sendLocation,myWork,&dataBase::locationClicked);
    connect(ui->locationButton,&QPushButton::clicked,myWork,[=](){
        emit sendLocation(ui->lineEdit3->text(),ui->lineEdit4->text());
    });
    connect(this,&MainWindow::sendTime,myWork,&dataBase::timeClicked);
    connect(ui->timeButton,&QPushButton::clicked,myWork,[=](){
        emit sendTime(ui->lineEdit5->text(),ui->lineEdit6->text());
    });
    connect(this,&MainWindow::sendLatitude,myWork,&dataBase::latitudeClicked);
    connect(ui->latitudeButton,&QPushButton::clicked,myWork,[=](){
        emit sendLatitude(ui->lineEdit7->text(),ui->lineEdit8->text());
    });
    connect(this,&MainWindow::sendLongitude,myWork,&dataBase::longitudeClicked);
    connect(ui->longitudeButton,&QPushButton::clicked,myWork,[=](){
        emit sendLongitude(ui->lineEdit9->text(),ui->lineEdit10->text());
    });
    connect(ui->filterButton,&QPushButton::clicked,myWork,&dataBase::filter);
    connect(ui->actionPlot,&QAction::triggered,this,[=](){
        dataPlot *myPlot=new dataPlot(this);
        myPlot->copyPOI(myWork->checkIn);
        if(myPlot->isEmpty())
            QMessageBox::information(this,tr(""),tr("No Data Exists"));
        else {
            myPlot->show();
        }
    });
    connect(ui->actionCompare,&QAction::triggered,this,[=]{
        comparePlot *compare=new comparePlot(this);
        compare->copyBase(myWork->db);
        compare->show();
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
