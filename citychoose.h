#ifndef CITYCHOOSE_H
#define CITYCHOOSE_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>
#include <QLabel>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include<QMouseEvent>
#include "mainweather.h"

namespace Ui {
class CityChoose;
}

class CityChoose : public QWidget
{
    Q_OBJECT

public:
    explicit CityChoose(QWidget *parent = nullptr);
    ~CityChoose();
    void CityShow(QJsonObject obj);
    void on_cityForm_entered(const QModelIndex &index);
    void weatherRequest();
    bool read_data(QNetworkReply *reply,int i);

private slots:
    void on_close_clicked();
    //由于去掉显示界面的菜单栏和标题栏，无法拖动窗口，重写关于窗口拖动用的几个函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QPoint last;//窗口拖动用变量
    QJsonObject jsonArray[4];
    Ui::CityChoose *ui;
    QNetworkAccessManager manager;
    QString key = "5390b9aaf9f7487dafa82735e929fcba";
};

#endif // CITYCHOOSE_H
