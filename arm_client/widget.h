#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include<QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include<QUdpSocket>

#include<QImage>
#include<QPainter>
#include<QCamera>
#include<QCameraImageCapture>
#include<QTimer>
#include<QDateTime>
#include<QCharRef>
typedef struct {
    QString msg1; //身份证uid
    QString msg2; //时间
    QString msg3; //温度
    QString msg4; //站点
    QString msg5;
    QString msg6;
}Udp_msg;

using namespace cv;
#define  card_BaudRate1 9600
#define  tem_BaudRate1 115200


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    bool camera_init(void);
    Mat QImage_to_cvMat(QImage image);
    QImage cvMat_to_Image(const cv::Mat& mat);
    bool process_image(Mat &mat);
    void paintEvent(QPaintEvent *event);
    void write_image(QImage &image,QString s1,QString s2);
    void image_timeout(void);

    void open_door(void);
    void close_door(void);

    void port_init(void);
    void card_serila_init(void);
    void temperature_serila_init(void);
    bool card_readData(void);
    bool tem_readData(void);

    void udp_init(void);
    void udp_emit(Udp_msg &msg);

    void door_timeout(void);
    void tem_timeout(void);

    QByteArray StringToHex(QString str);
    char ConvertHexChar(char ch);

    ~Widget();
public:
    Mat frame;//当前视频帧
    QImage *image;
    QImage static_image;
    CvCapture* capture;
    VideoCapture cap;
    QTimer* image_time;
    QTimer* tem_time;
    QTimer* door_time;
    bool image_tem_is_start;//温度采集和图像处理开始
    bool tem_is_ok;
    bool tem_read;
    bool is_use_static_image;

    QSerialPort *card_serial;//串口
    QSerialPort *tem_serial;//串口
    QString port_name[2];
    QStringList uid_receiverBuf;//一帧读卡 9个字节
    QStringList tem_receiverBuf;//一帧三个字节
    QString uid;
    QString tem;


    QUdpSocket *udpsocket;
    int udp_port;//udp端口参数
    bool udp_is_stated;
    Udp_msg msg;
    QString address;
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
