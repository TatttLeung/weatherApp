#include "mainweather.h"
#include "ui_mainweather.h"
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
#include <QPicture>
#include <QDateTime>
#include <QDebug>
#include<QJsonObject>
#include<QJsonArray>
#include<QMap>


MainWeather::MainWeather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWeather)
{
    ui->setupUi(this);
    this->setWindowTitle("天气查询详情");
    connect(ui->rebtn,&QPushButton::clicked,this,&MainWeather::reBack);

}

MainWeather::~MainWeather()
{
    delete ui;
}

void MainWeather::showobj(QJsonObject jsonArray[])
{
    //获取当前时间
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();
    //获取今天日期+时间
    QString today = dateTime.toString("yyyy-MM-dd hh:mm");
    //QString week1 = dateTime.toString("ddd");

    //获取五天的周几
    QString week2=dateTime.addDays(+1).toString("ddd");
    QString week3=dateTime.addDays(+2).toString("ddd");
    QString week4=dateTime.addDays(+3).toString("ddd");
    QString week5=dateTime.addDays(+4).toString("ddd");

    //获取五天的日期
    QString day1=dateTime.toString("MM月dd日");
    QString day2=dateTime.addDays(+1).toString("MM月dd日");
    QString day3=dateTime.addDays(+2).toString("MM月dd日");
    QString day4=dateTime.addDays(+3).toString("MM月dd日");
    QString day5=dateTime.addDays(+4).toString("MM月dd日");

    //设置时间
    ui->time->setPlainText(today);
    ui->date->setPlainText(day1);
    ui->date_1->setPlainText(day2);
    ui->date_2->setPlainText(day3);
    ui->date_3->setPlainText(day4);
    ui->date_4->setPlainText(day5);
    ui->today_2->setPlainText(week2);
    ui->today_3->setPlainText(week3);
    ui->today_4->setPlainText(week4);
    ui->today_5->setPlainText(week5);

    //实时天气赋值
    QJsonObject nowWeather = jsonArray[0].value("now").toObject();
    ui->temp->setPlainText(nowWeather.value("temp").toString()+"℃");    //温度
    ui->weather->setPlainText(nowWeather.value("text").toString());  //天气
    QString icon1 = ":/QWeather-Icons-1.1.1/icons/"+nowWeather.value("icon").toString()+".png";
    QImage Image1;
    Image1.load(icon1);
    QPixmap pixmap = QPixmap::fromImage(Image1);
    int with = ui->pic->width();
    int height = ui->pic->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->pic->setPixmap(fitpixmap);  //天气图片
    ui->wind->setPlainText(nowWeather.value("windDir").toString()); //风向
    ui->win_l->setPlainText(nowWeather.value("windScale").toString()+"级");  //风力等级
    ui->wet_l->setPlainText(nowWeather.value("humidity").toString()+"%");   //相对湿度
    //ui->sun_l->setPlainText(nowWeather.value("humidity").toString());
    ui->tep_l->setPlainText(nowWeather.value("feelsLike").toString()+"℃");  //体感温度
    ui->see_l->setPlainText(nowWeather.value("vis").toString()+"km");    //能见度
    ui->rain_l->setPlainText(nowWeather.value("precip").toString()+"mm");    //降水量
    ui->atmos_l->setPlainText(nowWeather.value("pressure").toString()+"hPa"); //大气压

    //生活指数赋值
    QJsonArray nowFeel  =jsonArray[2].value("daily").toArray();
    QJsonObject sport = nowFeel[0].toObject();
    ui->label->setText(sport.value("text").toString());
    QJsonObject sun = nowFeel[1].toObject();
    ui->sun_l->setPlainText(sport.value("category").toString());

    //空气质量赋值
    QJsonObject nowAirQuality = jsonArray[1].value("now").toObject();
    ui->AQI_v->setText(nowAirQuality.value("aqi").toString());
    ui->PM25_v->setPlainText(nowAirQuality.value("pm2p5").toString());
    ui->PM10_v->setPlainText(nowAirQuality.value("pm10").toString());
    ui->O3_v->setPlainText(nowAirQuality.value("o3").toString());
    ui->CO_v->setPlainText(nowAirQuality.value("co").toString());
    ui->SO2_v->setPlainText(nowAirQuality.value("so2").toString());
    ui->NO2_v->setPlainText(nowAirQuality.value("no2").toString());

    //五天天气预报赋值
    int with2 = ui->pic_2->width();
    int height2 = ui->pic_2->height();

    QJsonObject future[5];
    QJsonArray weatherArray = jsonArray[3].value("daily").toArray();
    for(int i=0;i<5;i++){
        future[i] = weatherArray[i].toObject();
    }

    QString icon2 = ":/QWeather-Icons-1.1.1/icons/"+future[0].value("iconDay").toString()+".png";
    QImage Image2;
    Image2.load(icon2);
    QPixmap pixmap2 = QPixmap::fromImage(Image2);
    QPixmap fitpixmap2 = pixmap2.scaled(with2, height2, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->pic_2->setPixmap(fitpixmap2);  //天气图片2

    QString icon3 = ":/QWeather-Icons-1.1.1/icons/"+future[1].value("iconDay").toString()+".png";
    QImage Image3;
    Image3.load(icon3);
    QPixmap pixmap3 = QPixmap::fromImage(Image3);
    QPixmap fitpixmap3 = pixmap3.scaled(with2, height2, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->pic_3->setPixmap(fitpixmap3);  //天气图片3

    QString icon4 = ":/QWeather-Icons-1.1.1/icons/"+future[2].value("iconDay").toString()+".png";
    QImage Image4;
    Image4.load(icon4);
    QPixmap pixmap4 = QPixmap::fromImage(Image4);
    QPixmap fitpixmap4 = pixmap4.scaled(with2, height2, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->pic_4->setPixmap(fitpixmap4);  //天气图片4

    QString icon5 = ":/QWeather-Icons-1.1.1/icons/"+future[3].value("iconDay").toString()+".png";
    QImage Image5;
    Image5.load(icon5);
    QPixmap pixmap5 = QPixmap::fromImage(Image5);
    QPixmap fitpixmap5 = pixmap5.scaled(with2, height2, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->pic_5->setPixmap(fitpixmap5);  //天气图片5

    QString icon6 = ":/QWeather-Icons-1.1.1/icons/"+future[4].value("iconDay").toString()+".png";
    QImage Image6;
    Image6.load(icon6);
    QPixmap pixmap6 = QPixmap::fromImage(Image6);
    QPixmap fitpixmap6 = pixmap6.scaled(with2, height2, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->pic_6->setPixmap(fitpixmap6);  //天气图片6

    ui->temp_2->setPlainText(future[0].value("tempMax").toString()+"℃");
    ui->temp_3->setPlainText(future[1].value("tempMax").toString()+"℃");
    ui->temp_4->setPlainText(future[2].value("tempMax").toString()+"℃");
    ui->temp_5->setPlainText(future[3].value("tempMax").toString()+"℃");
    ui->temp_6->setPlainText(future[4].value("tempMax").toString()+"℃");

    ui->temp_7->setPlainText(future[0].value("tempMin").toString()+"℃");
    ui->temp_8->setPlainText(future[1].value("tempMin").toString()+"℃");
    ui->temp_9->setPlainText(future[2].value("tempMin").toString()+"℃");
    ui->temp_10->setPlainText(future[3].value("tempMin").toString()+"℃");
    ui->temp_11->setPlainText(future[4].value("tempMin").toString()+"℃");

}

void MainWeather::reBack()
{
    CityCheck* cityCheck=new CityCheck();
    cityCheck->show();
    this->close();
}


