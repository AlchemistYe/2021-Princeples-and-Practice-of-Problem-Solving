#include "comparePlot.h"
#include "ui_comparePlot.h"

comparePlot::comparePlot(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::comparePlot)
{
    ui->setupUi(this);
    connect(ui->button1,&QPushButton::clicked,this,&comparePlot::userFilter);
    connect(ui->button2,&QPushButton::clicked,this,&comparePlot::locationFilter);
}

comparePlot::~comparePlot()
{
    delete ui;
}

void comparePlot::copyBase(QSqlDatabase &dataBase)
{
    db=dataBase;
}

void comparePlot::userPlot()
{
    int max1=0;
    for(int i=0;i<comPlot1.length();i++){
        if(max1<comPlot1[i].location)max1=comPlot1[i].location;
    }
    int *countTime1=new int[max1+1]{};
    int rank1[10];
    for(int i=0;i<10;i++)rank1[i]=-1;
    for(int i=0;i<comPlot1.length();i++){
        countTime1[comPlot1[i].location]++;
        int j=0;
        for(j=0;j<10;j++){
            if(j<=9&&rank1[j]==comPlot1[i].location)break;
        }
        if(j==10){
            j=9;
            while(j>=0&&rank1[j]==-1)j--;
            if(j<=8)rank1[j+1]=comPlot1[i].location;
            else{
                while(j>0&&countTime1[comPlot1[i].location]>countTime1[rank1[j]])j--;
                if(j<=8){
                for(int k=9;k>=j+2;k--)rank1[k]=rank1[k-1];
                rank1[j+1]=comPlot1[i].location;
                }
            }
        }
        else{
            while(j>0&&countTime1[comPlot1[i].location]>countTime1[rank1[j-1]]){
                rank1[j]=rank1[j-1];
                j--;
            }
            rank1[j]=comPlot1[i].location;
        }
    }
    if(rank1[9]==-1){
        QMessageBox::critical(0, qApp->tr("Error"),
                                            qApp->tr("Less Than 10 POIs"), QMessageBox::Cancel);
        return;
    }
    int max2=0;
    for(int i=0;i<comPlot2.length();i++){
        if(max2<comPlot2[i].location)max2=comPlot2[i].location;
    }
    int *countTime2=new int[max2+1]{};
    int rank2[10];
    for(int i=0;i<10;i++)rank2[i]=-1;
    for(int i=0;i<comPlot2.length();i++){
        countTime2[comPlot2[i].location]++;
        int j=0;
        for(j=0;j<10;j++){
            if(j<=9&&rank2[j]==comPlot2[i].location)break;
        }
        if(j==10){
            j=9;
            while(j>=0&&rank2[j]==-1)j--;
            if(j<=8)rank2[j+1]=comPlot2[i].location;
            else{
                while(j>0&&countTime2[comPlot2[i].location]>countTime2[rank2[j]])j--;
                if(j<=8){
                for(int k=9;k>=j+2;k--)rank2[k]=rank2[k-1];
                rank2[j+1]=comPlot2[i].location;
                }
            }
        }
        else{
            while(j>0&&countTime2[comPlot2[i].location]>countTime2[rank2[j-1]]){
                rank2[j]=rank2[j-1];
                j--;
            }
            rank2[j]=comPlot2[i].location;
        }
    }
    if(rank2[9]==-1){
        QMessageBox::critical(0, qApp->tr("Error"),
                                            qApp->tr("Less Than 10 POIs"), QMessageBox::Cancel);
        return;
    }
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QBarSet* set1 = new QBarSet(QString("User_id:%1").arg(comPlot1[0].user));
    QBarSet* set2 = new QBarSet(QString("User_id:%1").arg(comPlot2[0].user));
    for (int i = 0; i < 10; i++){
        *set1 << countTime1[rank1[i]];
        *set2 << countTime2[rank2[i]];
    }
    series->append(set1);
    series->append(set2);
    QStringList categories1;
    QStringList categories2;
    for (int i = 0; i < 10; i++)
    {
        categories1 << QString::number(rank1[i]);
        categories2 << QString::number(rank2[i]);
    }
    axis->append(categories1);
    axis->append(categories2);
    axis->setTitleText(QStringLiteral("Location ID"));
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setUseOpenGL(true);
    series->setLabelsVisible(true);
    QChart * chart = new QChart;
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addSeries(series);
    chart->setTitle("Top 10 Popular POIs of All Time");
    chart->addAxis(axis,Qt::AlignBottom);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    QChartView *chartView = new QChartView(this);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
    delete []countTime1;
    delete []countTime2;

}

void comparePlot::userFilter()
{
    comPlot1.clear();
    comPlot2.clear();
    QString begin=ui->lineEdit1->text();
    QString end=ui->lineEdit2->text();
    if(begin.length()>0&&end.length()>0){
        int b=begin.toInt();
        int e=end.toInt();
        if(b==e){
            QMessageBox::critical(0, qApp->tr("Error"),
                                  qApp->tr("The Same User"), QMessageBox::Cancel);
            return ;
        }
        QSqlQuery query1(NULL,db);
        query1.exec(QString("SELECT * FROM POI WHERE user==%1").arg(b));
        while(query1.next())
        {
            POI tmp;
            tmp.user = query1.value(0).toInt();
            tmp.location = query1.value(1).toInt();
            tmp.time = query1.value(2).toLongLong();
            tmp.latitude = query1.value(3).toDouble();
            tmp.longitude = query1.value(4).toDouble();
            comPlot1.append(tmp);
        }
        QSqlQuery query2(NULL,db);
        query2.exec(QString("SELECT * FROM POI WHERE user==%1").arg(e));
        while(query2.next())
        {
            POI tmp;
            tmp.user = query2.value(0).toInt();
            tmp.location = query2.value(1).toInt();
            tmp.time = query2.value(2).toLongLong();
            tmp.latitude = query2.value(3).toDouble();
            tmp.longitude = query2.value(4).toDouble();
            comPlot2.append(tmp);
        }
    }
    else{
        QMessageBox::critical(0, qApp->tr("Error"),
                              qApp->tr("Please Insert Two Users"), QMessageBox::Cancel);
        return;
    }
    if(comPlot1.length()==0||comPlot2.length()==0){
        QMessageBox::critical(0, qApp->tr("Error"),
                              qApp->tr("No Data In This Range"), QMessageBox::Cancel);
        return ;
    }
    userPlot();
}

void comparePlot::locationPlot(int b,int e)
{
    QTableWidget *table=new QTableWidget(this);
    table->setRowCount(first.length());
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList() << "Date" <<QString("DAU of Location_ID:%1").arg(b)
                                     <<"Checking-in times"<< QString("DAU of Location_ID:%1").arg(e)<<"Checking-in times");
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<first.length();i++){
        table->setItem(i,0,new QTableWidgetItem(QString("%1").arg(first[i])));
        table->setItem(i,1,new QTableWidgetItem(QString("%1").arg(DAU1[i])));
        table->setItem(i,2,new QTableWidgetItem(QString("%1").arg(time1[i])));
        table->setItem(i,3,new QTableWidgetItem(QString("%1").arg(DAU2[i])));
        table->setItem(i,4,new QTableWidgetItem(QString("%1").arg(time2[i])));
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    setCentralWidget(table);
}

void comparePlot::getDAU(qlonglong d,int b,int e)
{
    QSqlQuery query1(NULL,db);
    query1.exec(QString("SELECT * FROM POI WHERE location==%1 AND time>=%2 AND time<%3").arg(b).arg(d*1000000).arg((d+1)*1000000));
    int max=-1;
    QVector<int>DAU;
    while(query1.next())
    {
        int user=query1.value(0).toInt();
        DAU.append(user);
        if(user>max)max=user;
    }
    if(DAU.length()==0)return;
    int *countTime1=new int[max+1]{};
    for(int i=0;i<DAU.length();i++){
        countTime1[DAU[i]]++;
    }
    int m1=-1,index1=-1;
    for(int i=0;i<max+1;i++){
        if(countTime1[i]>m1){
            m1=countTime1[i];
            index1=i;
        }
    }
    DAU.clear();
    delete[] countTime1;
    QSqlQuery query2(NULL,db);
    query2.exec(QString("SELECT * FROM POI WHERE location==%1 AND time>=%2 AND time<%3").arg(e).arg(d*1000000).arg((d+1)*1000000));
    max=-1;
    while(query2.next())
    {
        int user=query2.value(0).toInt();
        DAU.append(user);
        if(user>max)max=user;
    }
    if(DAU.length()==0)return;
    int *countTime2=new int[max+1]{};
    for(int i=0;i<DAU.length();i++){
        countTime2[DAU[i]]++;
    }
    int m2=-1;int index2=-1;
    for(int i=0;i<max+1;i++){
        if(countTime2[i]>m2){
            m2=countTime2[i];
            index2=i;
        }
    }
    DAU2.append(index2);
    time2.append(m2);
    DAU1.append(index1);
    time1.append(m1);
    first.append(d);
    delete[] countTime2;
}


void comparePlot::locationFilter()
{
    DAU1.clear();
    DAU2.clear();
    first.clear();
    time1.clear();
    time2.clear();
    QString begin=ui->lineEdit3->text();
    QString end=ui->lineEdit4->text();
    if(begin.length()>0&&end.length()>0){
        int b=begin.toInt();
        int e=end.toInt();
        if(b==e){
            QMessageBox::critical(0, qApp->tr("Error"),
                                  qApp->tr("The Same User"), QMessageBox::Cancel);
            return ;
        }
        for(int i=2009;i<=2010;i++){
            for(int j=1;j<=12;j++){
                if(j==1||j==3||j==5||j==7||j==8||j==10||j==12){
                    for(int k=1;k<=31;k++)
                        getDAU(i*pow(10,4)+j*pow(10,2)+k,b,e);
                }
                if(j==2){
                    for(int k=1;k<=28;k++)
                        getDAU(i*pow(10,4)+j*pow(10,2)+k,b,e);
                }
                if(j==4||j==6||j==9||j==11){
                    for(int k=1;k<=30;k++)
                        getDAU(i*pow(10,4)+j*pow(10,2)+k,b,e);
                }
            }
        }
        if(first.length()>0){
            locationPlot(b,e);
        }
        else{
            QMessageBox::critical(0, qApp->tr("Error"),
                                  qApp->tr("No Data In This Range"), QMessageBox::Cancel);
        }
    }
    else{
        QMessageBox::critical(0, qApp->tr("Error"),
                              qApp->tr("Please Insert Two Users"), QMessageBox::Cancel);
        return;
    }
}
