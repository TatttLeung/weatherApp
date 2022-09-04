#include "citychoose.h"
#include "ui_citychoose.h"
#include <QPushButton>
#include<QDebug>
#include<QHBoxLayout>

CityChoose::CityChoose(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CityChoose)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏窗口框
}

CityChoose::~CityChoose()
{
    delete ui;
}

/**
 * 将获取到的城市信息让用户进行选择
 * @brief CityChoose::CityShow
 * @param obj
 */

void CityChoose::CityShow(QJsonObject obj)
{

    //设置列内容自适应宽度
    ui->cityForm->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //开启交替行背景色,在设置style为交替颜色时必须开启
    ui->cityForm->setAlternatingRowColors(true);
    //不允许编辑
    ui->cityForm->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //行选择
    ui->cityForm->setSelectionBehavior(QAbstractItemView::SelectRows);
    //列自适应宽度
    ui->cityForm->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //将地区返回数据成为数组
    QJsonArray cityArray = obj.value("location").toArray();
    ui->cityForm->setRowCount(cityArray.size());
    for(int i=0;i<cityArray.size();i++)
    {
        int col=0;
        QJsonObject cityobj = cityArray.at(i).toObject();
        ui->cityForm->setItem(i,col++,new QTableWidgetItem(cityobj.value("id").toString()));
        ui->cityForm->setItem(i,col++,new QTableWidgetItem(cityobj.value("name").toString()));
        ui->cityForm->setItem(i,col++,new QTableWidgetItem(cityobj.value("adm2").toString()));
        ui->cityForm->setItem(i,col++,new QTableWidgetItem(cityobj.value("adm1").toString()));

        //添加选择按钮
        auto button = new QPushButton(QStringLiteral("选择"), ui->cityForm);
        button->setFixedSize(60, 22);
        auto cellWidget = new QWidget(this);
        auto layout = new QHBoxLayout(cellWidget);
        // 设置水平居中
        layout->setAlignment(cellWidget, Qt::AlignCenter);
        // 设置部件周围的左、上、右、下边距
        layout->setContentsMargins(10, 0, 10, 0);
        layout ->addWidget(button);
        cellWidget->setLayout(layout);
        ui->cityForm->setCellWidget(i,col, cellWidget);
        //添加信号槽函数
       connect(button,&QPushButton::clicked,this,&CityChoose::weatherRequest);
    }

}

/**
 * 选择按钮槽函数
 * @brief CityChoose::weatherRequest
 */
void CityChoose::weatherRequest()
{
    // 获取按钮的指针
    QPushButton *pushButton_ = dynamic_cast<QPushButton*>(this->sender());
    if(NULL == pushButton_)
    {
        return;
    }
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->cityForm->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int row = index.row();
    //获取地区ID
    qDebug()<<ui->cityForm->item(row ,0)->text()<<endl;
    QString location = ui->cityForm->item(row ,0)->text();

    //获取ID后，发送查询实时天气请求、7天天气预报请求、空气质量请求和生活指数请求
    QString ip[4];
    ip[0] = "https://devapi.qweather.com/v7/weather/now?key="+key+"&location="+location; //天气请求
    ip[1] = "https://devapi.qweather.com/v7/air/now?key="+key+"&location="+location; //空气质量请求
    ip[2] = "https://devapi.qweather.com/v7/indices/1d?key="+key+"&location="+location+"&type=1,5"; //生活指数请求
    ip[3] = "https://devapi.qweather.com/v7/weather/7d?key="+key+"&location="+location; //7天天气预报
    //天气url
    QUrl url[4];
    for(int i=0;i<4;i++)
    {
        url[i] = QUrl(ip[i]);
    }
    //创建一个请求对象
    QNetworkRequest request[4];
    bool flag=true;
    for(int i=0;i<4;i++)
    {
        //添加事件循环机制，让其异步请求获取完后再调用read_data函数
        QEventLoop eventLoop;
        request[i] = QNetworkRequest(url[i]);
        QNetworkReply *reply = manager.get(request[i]);
        connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();       //block until finish
        flag *= read_data(reply,i);
    }

    if(flag)
    {
        //显示出来
        MainWeather* mainweather=new MainWeather();
        mainweather->showobj(jsonArray);
        mainweather->show();
        this->close();
    }
    else
    {
        flag=true;
    }
}


/**
 * 解析获取到的数据
 * @brief CityChoose::read_data
 * @param reply
 */
bool CityChoose::read_data(QNetworkReply *reply,int i)
{
    QByteArray array = reply->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(array,&error);
    qDebug()<<array.data()<<endl;

    if(error.error !=QJsonParseError::NoError)
    {
        qDebug("josn error");
        return false;
    }

    QJsonObject obj = doc.object();
    if(obj.value("code").toString()=="200")
    {
        //如果请求成功，则解析JSON数据
        qDebug()<<i<<endl;
        jsonArray[i] = obj;
        return true;
    }
    else if(obj.value("code").toString()=="204"||obj.value("code").toString()=="404")
    {
        //404或者204代码返回时，代表没有这个地区相关
        QMessageBox::information(this,"info","无此地区信息");
        qDebug()<<"无此地区信息"<<endl;
        return false;
    }
    else
    {
        QMessageBox::critical(this,"critical","系统出错！请联系管理员");
        return false;
    }



}

//三个鼠标事件的重写
//鼠标释放
void CityChoose::mousePressEvent(QMouseEvent *e)
{
    if(e->button()== Qt::LeftButton)	//鼠标左键释放
        last = e->globalPos();
}
//鼠标移动
void CityChoose::mouseMoveEvent(QMouseEvent *e)
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
void CityChoose::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)	//鼠标左键按下
    {
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        move(x()+dx, y()+dy);
    }

}

void CityChoose::on_close_clicked()
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

