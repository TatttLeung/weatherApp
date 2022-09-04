#ifndef MAINWEATHER_H
#define MAINWEATHER_H
#include "citycheck.h"

#include <QWidget>

namespace Ui {
class MainWeather;
}

class MainWeather : public QWidget
{
    Q_OBJECT

public:
    explicit MainWeather(QWidget *parent = nullptr);
    ~MainWeather();
    void showobj(QJsonObject jsonArray[]);
    void reBack();
private:
    Ui::MainWeather *ui;
};

#endif // MAINWEATHER_H
