#include "dataBase.h"

dataBase::dataBase(QWidget *parent) : QWidget(parent)
{

}

qlonglong dataBase::toLongInt(QString time)
{    QByteArray input=time.toUtf8();
     qlonglong t=0;
     for(int i=0;i<19;i++){
         if(i==4||i==7||i==10||i==13||i==16||i==19)continue;
         if(i>=0&&i<4)t+=(int(input.at(i))-'0')*pow(10,13-i);
         else if(i==5||i==6)t+=(int(input.at(i))-'0')*pow(10,14-i);
         else if(i==8||i==9)t+=(int(input.at(i))-'0')*pow(10,15-i);
         else if(i==11||i==12)t+=(int(input.at(i))-'0')*pow(10,16-i);
         else if(i==14||i==15)t+=(int(input.at(i))-'0')*pow(10,17-i);
         else if(i==17||i==18)t+=(int(input.at(i))-'0')*pow(10,18-i);
     }
      return t;
}

void dataBase::getPOI(QString name)
{
    fileName=name;
    QFile excel(fileName);
    if(!excel.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,tr("open .csv"),tr("can't open %1:\n").arg(fileName));
        QApplication::exit(0);
    }
    checkIn.clear();
    int row=0;
    QTextStream input(&excel);
    QStringList list;
    while(!input.atEnd()){
        QString excelLine=input.readLine();
        list=excelLine.split(",");
        POI temp;
        temp.user=list.at(0).toInt();
        temp.location=list.at(1).toInt();
        temp.time=toLongInt(list.at(2));
        temp.latitude=list.at(3).toDouble();
        temp.longitude=list.at(4).toDouble();
        checkIn.append(temp);
        row++;
        emit sendRow(row);
    }
    beginPOI.user=0;
    endPOI.user=INT_MAX;
    beginPOI.location=0;
    endPOI.location=INT_MAX;
    beginPOI.time=0;
    endPOI.time=LLONG_MAX;
    beginPOI.latitude=-180;
    endPOI.latitude=180;
    beginPOI.longitude=-180;
    endPOI.longitude=180;
    excel.close();
    QMessageBox::information(this,tr("%1").arg(fileName),tr("Successfully Uploaded"));
}

bool dataBase::isEmpty()
{
   if(checkIn.length()==0)return 1;
   else return 0;
}



void dataBase::createTable()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
           QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                 qApp->tr("Unable to establish database connection."), QMessageBox::Cancel);

        }
    QSqlQuery query;
    QString createSql=QString("CREATE TABLE POI(user INTEGER NOT NULL,location INTEGER NOT NULL,"
                              "time INTEGER NOT NULL,latitude DOUBLE NOT NULL, longitude DOUBLE NOT NULL)");
    query.prepare(createSql);
    query.exec();
    if(db.transaction()){
        int i=0;
        while(i<checkIn.length()){
            query.prepare("INSERT INTO POI(user,location,time,latitude,longitude)"
                          "VALUES(:user,:location,:time,:latitude,:longitude)");
            query.bindValue(0,checkIn[i].user);
            query.bindValue(1,checkIn[i].location);
            query.bindValue(2,checkIn[i].time);
            query.bindValue(3,checkIn[i].latitude);
            query.bindValue(4,checkIn[i].longitude);
            query.exec();
            i++;
        }
        db.commit();
    }
}

void dataBase::userClicked(QString begin, QString end)
{
   if(begin.length()>0&&end.length()>0){
       int b=begin.toInt();
       int e=end.toInt();
       if(b>e){QMessageBox::critical(0, qApp->tr("Error"),
                                    qApp->tr("Unable to save"), QMessageBox::Cancel);
           return ;
       }
       else {
             beginPOI.user=b;
             endPOI.user=e;
       }
   }
   else if(begin.length()>0){
        int b=begin.toInt();
        beginPOI.user=b;
        endPOI.user=INT_MAX;
   }
   else if(end.length()>0){
       int e=end.toInt();
       beginPOI.user=0;
       endPOI.user=e;
   }
   else {
       beginPOI.user=0;
       endPOI.user=INT_MAX;
   }
}

void dataBase::locationClicked(QString begin, QString end)
{
  if(begin.length()>0&&end.length()>0){
      int b=begin.toInt();
      int e=end.toInt();
      if(b>e){QMessageBox::critical(0, qApp->tr("Error"),
                                    qApp->tr("Unable to save"), QMessageBox::Cancel);
           return ;
       }
      else {
          beginPOI.location=b;
          endPOI.location=e;
      }
  }
  else if(begin.length()>0){
       int b=begin.toInt();
       beginPOI.location=b;
       endPOI.location=INT_MAX;
  }
  else if(end.length()>0){
      beginPOI.location=0;
      int e=end.toInt();
      endPOI.location=e;
  }
  else {
      beginPOI.location=0;
      endPOI.location=INT_MAX;
  }
}

void dataBase::timeClicked(QString begin, QString end)
{
    if(begin.length()>0&&end.length()>0){
        qlonglong b=begin.toLongLong();
        qlonglong e=end.toLongLong();
        if(b>e){QMessageBox::critical(0, qApp->tr("Error"),
                                      qApp->tr("Unable to save"), QMessageBox::Cancel);
             return ;
        }
        else{
            beginPOI.time=b;
            endPOI.time=e;
        }
    }
    else if(begin.length()>0){
        qlonglong b=begin.toLongLong();
        beginPOI.time=b;
        endPOI.time=LLONG_MAX;
    }
    else if(end.length()>0){
        beginPOI.time=0;
        qlonglong e=end.toLongLong();
        endPOI.time=e;
    }
    else {
        beginPOI.time=0;
        endPOI.time=LLONG_MAX;
    }
}

void dataBase::latitudeClicked(QString begin, QString end)
{
    if(begin.length()>0&&end.length()>0){
        double b=begin.toDouble();
        double e=end.toDouble();
        if(b>e){QMessageBox::critical(0, qApp->tr("Error"),
                                      qApp->tr("Unable to save"), QMessageBox::Cancel);
             return ;
        }
        else{
            beginPOI.latitude=b;
            endPOI.latitude=e;
        }
    }
    else if(begin.length()>0){
        double b=begin.toDouble();
        beginPOI.latitude=b;
        endPOI.latitude=180;
    }
    else if(end.length()>0){
        beginPOI.latitude=-180;
        double e=end.toDouble();
        endPOI.latitude=e;
    }
    else {
        beginPOI.latitude=-180;
        endPOI.latitude=180;
    }
}

void dataBase::longitudeClicked(QString begin, QString end)
{
    if(begin.length()>0&&end.length()>0){
        double b=begin.toDouble();
        double e=end.toDouble();
        if(b>e){QMessageBox::critical(0, qApp->tr("Error"),
                                      qApp->tr("Unable to save"), QMessageBox::Cancel);
             return ;
        }
        else{
            beginPOI.longitude=b;
            endPOI.longitude=e;
        }
    }
    else if(begin.length()>0){
        double b=begin.toDouble();
        beginPOI.longitude=b;
        endPOI.longitude=180;
    }
    else if(end.length()>0){
        beginPOI.longitude=-180;
        double e=end.toDouble();
        endPOI.longitude=e;
    }
    else {
        beginPOI.longitude=-180;
        endPOI.longitude=180;
    }
}

void dataBase::filter()
{
    checkIn.clear();
    QSqlQuery query(NULL,db);
    query.exec(QString("SELECT * FROM POI WHERE user>=%1 AND user<=%2 AND location>=%3 AND location<=%4 AND time>=%5 AND time<=%6 AND latitude>=%7 AND latitude<=%8 AND longitude>=%9 AND longitude<=%10 ")
               .arg(beginPOI.user).arg(endPOI.user).arg(beginPOI.location).arg(endPOI.location)
               .arg(beginPOI.time).arg(endPOI.time).arg(beginPOI.latitude).arg(endPOI.latitude)
               .arg(beginPOI.longitude).arg(endPOI.longitude));
    while (query.next())
    {
        POI tmp;
        tmp.user = query.value(0).toInt();
        tmp.location = query.value(1).toInt();
        tmp.time = query.value(2).toLongLong();
        tmp.latitude = query.value(3).toDouble();
        tmp.longitude = query.value(4).toDouble();
        checkIn.append(tmp);
    }
    if(checkIn.length()>0){
        emit sendPOI(checkIn);
        QMessageBox::information(0, qApp->tr(""),
                                 qApp->tr("Filtering Completed"), QMessageBox::Cancel);
    }
    else  QMessageBox::critical(0, qApp->tr("Error"),
                                qApp->tr("No Data In This Range"), QMessageBox::Cancel);
}


