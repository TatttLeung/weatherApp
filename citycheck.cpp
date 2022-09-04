#include "citycheck.h"
#include "ui_citycheck.h"

#include <QDebug>

CityCheck::CityCheck(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CityCheck)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏窗口框

    //连接查询按钮和查询函数
    connect(ui->CityBtn,&QPushButton::clicked,this,&CityCheck::checkCity);
    //关联finished信号
    connect(&manager,&QNetworkAccessManager::finished,this,&CityCheck::read_data);
}

CityCheck::~CityCheck()
{
    delete ui;
}

/**
 * 查询城市按钮点击后发送请求
 * @brief CityCheck::checkCity
 */
void CityCheck::checkCity()
{
    QString ip;
    //通过获取城市输入框的当前值来选择哪个城市的URL
    QString city = ui->City->text();
    if(!city.isEmpty())
    {
        ip = "https://geoapi.qweather.com/v2/city/lookup?key="+key+"&location="+city+"&range=cn";
        //天气url
        QUrl url(ip);
        //创建一个请求对象
        QNetworkRequest request(url);
        manager.get(request);
    }
}

/**
 * 解析城市接口获取到的数据
 * @brief CityCheck::read_data
 * @param reply
 */
void CityCheck::read_data(QNetworkReply *reply)
{
    QByteArray array = reply->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(array,&error);
    qDebug()<<array.data()<<endl;

    if(error.error !=QJsonParseError::NoError)
    {
        qDebug("josn error");
        return ;
    }

    QJsonObject obj = doc.object();
    if(obj.value("code").toString()=="200")
    {
        //如果请求成功，则解析JSON数据
        //用指针，防止析构函数导致一闪而过
        CityChoose* citychoose=new CityChoose();
        citychoose->CityShow(obj);
        citychoose->show();
        this->close();

    }
    else if(obj.value("code").toString()=="204"||obj.value("code").toString()=="404")
    {
        //404或者204代码返回时，代表没有这个地区
        QMessageBox::information(this,"info","无此地区信息");
        qDebug()<<"无地区信息"<<endl;
    }
    else
    {
        QMessageBox::critical(this,"critical","系统出错！请联系管理员");
    }



}

//三个鼠标事件的重写
//鼠标释放
void CityCheck::mousePressEvent(QMouseEvent *e)
{
    if(e->button()== Qt::LeftButton)	//鼠标左键释放
        last = e->globalPos();
}
//鼠标移动
void CityCheck::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()==Qt::LeftButton)	//坐标左键按下并移动
    {
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
    }

}
//鼠标按下
void CityCheck::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)	//鼠标左键按下
    {
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        move(x()+dx, y()+dy);
    }

}



void CityCheck::on_close_clicked()
{
    QMessageBox MBox;
    MBox.setWindowTitle("退出程序");
    MBox.setText("确认退出程序？");
    QPushButton *agreeBut1 = MBox.addButton("确认", QMessageBox::AcceptRole);
    QPushButton *agreeBut2 = MBox.addButton("取消", QMessageBox::RejectRole);
    MBox.exec();
    if (MBox.clickedButton() == (QAbstractButton*)agreeBut1) {
        //在 Qt Creator 的输出窗口中输出指定字符串
         this->close();
    }
    else
    {
        return; // 忽略退出信号，程序继续进行
    }

}

