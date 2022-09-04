#ifndef CITYCHECK_H
#define CITYCHECK_H

#include <QMainWindow>
#include <QSslSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>
#include <QLabel>
#include <QTimer>
#include<QMouseEvent>
#include "citychoose.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CityCheck; }
QT_END_NAMESPACE

class CityCheck : public QMainWindow
{
    Q_OBJECT

public:
    CityCheck(QWidget *parent = nullptr);
    ~CityCheck();

private slots:
    void read_data(QNetworkReply *reply);
    void checkCity();
    //由于去掉显示界面的菜单栏和标题栏，无法拖动窗口，重写关于窗口拖动用的几个函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void on_close_clicked();

private:
    QPoint last;//窗口拖动用变量
    Ui::CityCheck *ui;
    QNetworkAccessManager manager;
    QString key = "5390b9aaf9f7487dafa82735e929fcba";
};
#endif // CITYCHECK_H
