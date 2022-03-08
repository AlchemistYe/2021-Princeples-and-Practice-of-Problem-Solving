#include "dataPlot.h"
#include "ui_dataPlot.h"

dataPlot::dataPlot(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::dataPlot)
{
    ui->setupUi(this);
    connect(ui->button1,&QPushButton::clicked,this,&dataPlot::topTen1);
    connect(ui->button3,&QPushButton::clicked,this,&dataPlot::topTen2);
    connect(ui->button2,&QPushButton::clicked,this,&dataPlot::getNum1);
    connect(ui->button4,&QPushButton::clicked,this,&dataPlot::getNum2);


}

dataPlot::~dataPlot()
{
    delete ui;
}

void dataPlot::copyPOI(QVector<POI>checkIn)
{
    plotOut=checkIn;
}


bool dataPlot::isEmpty()
{
    if(plotOut.length()==0)return 1;
    else return 0;
}

void dataPlot::topTen1()
{
   int max=0;
   for(int i=0;i<plotOut.length();i++){
       if(max<plotOut[i].location)max=plotOut[i].location;
   }
   int *countTime=new int[max+1]{};
   int rank[10];
   for(int i=0;i<10;i++)rank[i]=-1;
   for(int i=0;i<plotOut.length();i++){
       countTime[plotOut[i].location]++;
       int j=0;
       for(j=0;j<10;j++){
           if(j<=9&&rank[j]==plotOut[i].location)break;
       }
       if(j==10){
           j=9;
           while(j>=0&&rank[j]==-1)j--;
           if(j<=8)rank[j+1]=plotOut[i].location;
           else{
               while(j>0&&countTime[plotOut[i].location]>countTime[rank[j]])j--;
               if(j<=8){
               for(int k=9;k>=j+2;k--)rank[k]=rank[k-1];
               rank[j+1]=plotOut[i].location;
               }
           }
       }
       else{
           while(j>0&&countTime[plotOut[i].location]>countTime[rank[j-1]]){
               rank[j]=rank[j-1];
               j--;
           }
           rank[j]=plotOut[i].location;
       }
   }
   if(rank[9]==-1){
       QMessageBox::critical(0, qApp->tr("Error"),
                                           qApp->tr("Less Than 10 POIs"), QMessageBox::Cancel);
       return;
   }
   QBarSeries *series = new QBarSeries();
   QBarCategoryAxis *axis = new QBarCategoryAxis();
   QBarSet* set = new QBarSet("Number of Checking-ins");
   for (int i = 0; i < 10; i++)
       *set << countTime[rank[i]];
   series->append(set);
   QStringList categories;
   for (int i = 0; i < 10; i++)
   {
       categories << QString::number(rank[i]);
   }   
   axis->append(categories);
   axis->setTitleText(QStringLiteral("Location ID"));
   series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
   series->setUseOpenGL(true);
   series->setLabelsVisible(true);
   QChart * chart = new QChart;
   chart->setAnimationOptions(QChart::SeriesAnimations);
   chart->addSeries(series);
   chart->setTitle("Top 10 Popular POIs");
   chart->addAxis(axis,Qt::AlignBottom);
   chart->legend()->setVisible(true);
   chart->legend()->setAlignment(Qt::AlignBottom);
   chart->setTheme(QChart::ChartThemeBlueCerulean);
   QChartView *chartView = new QChartView(this);
   chartView->setChart(chart);
   chartView->setRenderHint(QPainter::Antialiasing);
   setCentralWidget(chartView);
   delete []countTime;
}

void dataPlot::topTen2()
{
    int max=0;
    for(int i=0;i<plotOut.length();i++){
        if(max<plotOut[i].location)max=plotOut[i].location;
    }
    int *countTime=new int[max+1]{};
    int rank[10];
    for(int i=0;i<10;i++)rank[i]=-1;
    for(int i=0;i<plotOut.length();i++){
        countTime[plotOut[i].location]++;
        int j=0;
        for(j=0;j<10;j++){
            if(j<=9&&rank[j]==plotOut[i].location)break;
        }
        if(j==10){
            j=9;
            while(j>=0&&rank[j]==-1)j--;
            if(j<=8)rank[j+1]=plotOut[i].location;
            else{
                while(j>0&&countTime[plotOut[i].location]>countTime[rank[j]])j--;
                if(j<=8){
                for(int k=9;k>=j+2;k--)rank[k]=rank[k-1];
                rank[j+1]=plotOut[i].location;
                }
            }
        }
        else{
            while(j>0&&countTime[plotOut[i].location]>countTime[rank[j-1]]){
                rank[j]=rank[j-1];
                j--;
            }
            rank[j]=plotOut[i].location;
        }
    }
    if(rank[9]==-1){
        QMessageBox::critical(0, qApp->tr("Error"),
                                            qApp->tr("Less Than 10 POIs"), QMessageBox::Cancel);
        return;
    }
    QPieSeries *series = new QPieSeries();
    for(int i = 0; i < 10; i++)
        series->append(QString("%1").arg(rank[i]),countTime[rank[i]]);
    series->setLabelsVisible();
    QVector<QPieSlice*>slice;
    for(int i=0;i<10;i++){
        slice.append(series->slices().at(i));
        slice[i]->setExploded();
        slice[i]->setExplodeDistanceFactor(0.1);
    }
    QChart * chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addSeries(series);
    chart->setTitle("Top 10 Popular POIs");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartView = new QChartView(this);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
    delete []countTime;
}

void dataPlot::getNum1()
{
   qlonglong min=20200000000000,max=0;
   for(int i=0;i<plotOut.length();i++){
       if(plotOut[i].time>max){
           max=plotOut[i].time;
       }
       if(plotOut[i].time<min){
           min=plotOut[i].time;
       }
   }
   min=min/100000000;
   max=max/100000000;
   int number;
   if(min/100==2009&&max/100==2010){
       number=200913-min+max-201000;
   }
   else{
       number=max-min+1;
   }
   int *countTime=new int[number]{};
   bool specific=true;
   int location=plotOut[0].location;
   for(int i=0;i<plotOut.length();i++){
       if(plotOut[i].location!=location)specific=false;
       qlonglong subtime=plotOut[i].time/100000000;
       if(min/100==2009&&subtime/100==2010){
           countTime[200913-min+subtime-201001]++;
       }
       else{
           countTime[subtime-min]++;
       }
   }
   QStringList categories;
   for (int i = 0; i < number; i++)
   {
       if(min/100==2009&&max/100==2010){
         if(min+i<=200912)
             categories<<QString::number(min+i);
         else
             categories<<QString::number(201000+i-(200912-min));

       }
       else{
           categories<<QString::number(min+i);
       }
   }
   QBarSeries *series = new QBarSeries();
   QBarCategoryAxis *axis = new QBarCategoryAxis();
   QBarSet* set = new QBarSet("Number of Checking-ins");
   for(int i=0;i<number;i++)
       *set<<countTime[i];
   series->append(set);   
   axis->append(categories);
   axis->setTitleText(QStringLiteral("Year+Month"));
   series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
   series->setUseOpenGL(true);
   series->setLabelsVisible(true);
   QChart * chart = new QChart;
   chart->setAnimationOptions(QChart::SeriesAnimations);
   chart->addSeries(series);
   if(specific){
       chart->setTitle("A Specific POI");
   }
   else{
       chart->setTitle("GPS Range Search");
   }
   chart->addAxis(axis,Qt::AlignBottom);
   chart->legend()->setVisible(true);
   chart->legend()->setAlignment(Qt::AlignBottom);
   chart->setTheme(QChart::ChartThemeBlueCerulean);
   QChartView *chartView = new QChartView(this);
   chartView->setChart(chart);
   chartView->setRenderHint(QPainter::Antialiasing);
   setCentralWidget(chartView);
   delete []countTime;
}

void dataPlot::getNum2()
{
    qlonglong min=20200000000000,max=0;
    for(int i=0;i<plotOut.length();i++){
        if(plotOut[i].time>max){
            max=plotOut[i].time;
        }
        if(plotOut[i].time<min){
            min=plotOut[i].time;
        }
    }
    min=min/100000000;
    max=max/100000000;
    int number;
    if(min/100==2009&&max/100==2010){
        number=200913-min+max-201000;
    }
    else{
        number=max-min+1;
    }
    int *countTime=new int[number]{};
    bool specific=true;
    int location=plotOut[0].location;
    for(int i=0;i<plotOut.length();i++){
        if(plotOut[i].location!=location)specific=false;
        qlonglong subtime=plotOut[i].time/100000000;
        if(min/100==2009&&subtime/100==2010){
            countTime[200913-min+subtime-201001]++;
        }
        else{
            countTime[subtime-min]++;
        }
    }
    QStringList categories;
    for (int i = 0; i < number; i++)
    {
        if(min/100==2009&&max/100==2010){
          if(min+i<=200912)
              categories<<QString::number(min+i);
          else
              categories<<QString::number(201000+i-(200912-min));

        }
        else{
            categories<<QString::number(min+i);
        }
    }
    QSplineSeries *splineSeries = new QSplineSeries();
    for(int i=0;i<number;i++){
        splineSeries->append(i,countTime[i]);
    }
    QScatterSeries *scatterSeries = new QScatterSeries();
    scatterSeries->setPointLabelsFormat("@yPoint");
    scatterSeries->setPointLabelsVisible();
    scatterSeries->setMarkerSize(7);
    for(int i=0;i<number;i++){
        scatterSeries->append(i,countTime[i]);
    }
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(splineSeries);
    chart->addSeries(scatterSeries);
    if(specific){
        chart->setTitle("A Specific POI");
    }
    else{
        chart->setTitle("GPS Range Search");
    }
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->createDefaultAxes();
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->addAxis(axis, Qt::AlignBottom);
    scatterSeries->attachAxis(axis);
    QChartView *chartView = new QChartView(this);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
    delete []countTime;
}

