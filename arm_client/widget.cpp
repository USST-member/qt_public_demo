#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(" ");
    this->msg.msg1="图书馆";
    this->tem_is_ok=false;
    this->tem_read=false;
    this->image_tem_is_start=false;
    this->udp_is_stated=false;
    this->is_use_static_image=false;
    image=new QImage;
    camera_init();
    udp_init();
    port_init();
    image_time=new QTimer(this);//用于抓取图像的时间间隔
    image_time->start(200);


    tem_time=new QTimer(this);//用于抓取温度的时间间隔

    door_time=new QTimer(this);



    connect(this->image_time, &QTimer::timeout, this, &Widget::image_timeout);
    connect(this->door_time, &QTimer::timeout, this, &Widget::door_timeout);
    connect(this->tem_time, &QTimer::timeout, this, &Widget::tem_timeout);
//    connect(this->card_serial, &QSerialPort::readyRead, this, &Widget::card_readData);
//    connect(this->tem_serial, &QSerialPort::readyRead, this, &Widget::tem_readData);
    /*接受数据之后的数据处理尚待解决*/

}


void Widget::tem_timeout(void)
{
    /*发送读取温度命令*/
    QByteArray data=StringToHex("b");
    tem_serial->write(data);
    tem_serial->write(data);
}

void Widget::open_door(void)
{
    this->door_time->start(3000);
}
void Widget::close_door(void)
{
    this->udp_is_stated=true;
    this->udp_emit(msg);
    image_tem_is_start=false;
    tem_is_ok=false;
    udp_is_stated=false;
    tem_time->stop();
    tem_read=false;
    this->is_use_static_image=false;
}

void Widget::door_timeout(void)
{
    this->door_time->stop();
    this->close_door();
}
bool Widget::tem_readData(void)
{
    if(image_tem_is_start==true)
    {
        QByteArray buf;
        buf=tem_serial->readAll();
        tem_receiverBuf.append(buf);

        /*根据手册温度返回值为三个字节*/
        if(tem_receiverBuf.size()==3)
        {
            tem=QString("%1, %2").arg(uid_receiverBuf.at(0)).arg(uid_receiverBuf.at(1));
            QString s(((tem.toInt()-1000)/10));
            tem=s;
            this->msg.msg3=tem;
            if(tem.toInt()<30||tem.toInt())
            {
                write_image(*image,"温度异常"," ");
                this->static_image=*image;
                this->is_use_static_image=true;
            }
            else
            {
                tem_is_ok=true;
                qDebug()<<tem;
                tem_receiverBuf.clear();
                return true;
            }
        }
    }
    else
        return false;
}

bool Widget::card_readData(void)
{
    QByteArray buf;
    buf=card_serial->readAll();
    uid_receiverBuf.append(buf.toHex());

    /*根据手册一帧长度为9，第一个字节为0x02,最后一个字节为0x03*/
    if(uid_receiverBuf.at(0)==0x02&&uid_receiverBuf.size()==9&&uid_receiverBuf.at(2)==0x04&&uid_receiverBuf.at(8)==0x03)
    {
        uid=QString("%1, %2, %3,%4").arg(uid_receiverBuf.at(3).toStdWString()).arg(uid_receiverBuf.at(4).toStdWString()).arg(uid_receiverBuf.at(5).toStdWString()).arg(uid_receiverBuf.at(6).toStdWString());
        this->msg.msg1=uid;
        this->image_tem_is_start=true;//识别出卡号，开始图片处理和温度采集
        qDebug()<<uid;
        uid_receiverBuf.clear();
        return true;
    }
    else
    {
        return false;
    }

}

/*两个串口的初始化*/
void Widget::port_init(void)
{
    QStringList list;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            list<<info.portName();
        }

    if(list.size()==2)
    {
        port_name[0]=list[0];
        port_name[1]=list[1];
        this->card_serila_init();
        this->temperature_serila_init();
    }

    return;
}
/*udp传输的初始化*/
void Widget::udp_init(void)
{
    this->udp_port=22;
    this->udpsocket=new QUdpSocket(this);
    this->udp_is_stated=false;
    this->address="192.168.13.1";
    this->msg.msg1="食堂";
    this->msg.msg2="wqwq";
    this->msg.msg3="treetretr";
    this->msg.msg4="rteret";
    this->msg.msg5="reg";
    this->msg.msg6="gre";
}


/*udp传输数据*/
void  Widget::udp_emit(Udp_msg &msg)
{
    QString str = QString("%1, %2, %3,%4,%5,%6").arg(msg.msg1).arg(msg.msg2).arg(msg.msg3).arg(msg.msg4).arg(msg.msg5).arg(msg.msg6);
    // 参数1是发送的数据，最好用QByteArray，参数2是对方地址，参数3是对方接收端口号
    udpsocket->writeDatagram(QByteArray( str.toLatin1()),QHostAddress(this->address),this->udp_port);
    return ;
}
/*读卡串口的初始化*/
void Widget::card_serila_init(void)
{
    card_serial = new QSerialPort;
    //设置串口名
    card_serial->setPortName(port_name[0]);
    //打开串口
    card_serial->open(QIODevice::ReadWrite);
    //设置波特率
    card_serial->setBaudRate(card_BaudRate1);
    //设置数据位数
    card_serial->setDataBits(QSerialPort::Data8);
     //设置奇偶校验
    card_serial->setParity(QSerialPort::NoParity);
    //设置停止位
    card_serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    card_serial->setFlowControl(QSerialPort::NoFlowControl);
}

/*温度串口的初始化*/
void Widget::temperature_serila_init(void)
{

    tem_serial = new QSerialPort;
    //设置串口名
    tem_serial->setPortName(port_name[1]);
    //打开串口
    tem_serial->open(QIODevice::ReadWrite);
    //设置波特率
    tem_serial->setBaudRate(tem_BaudRate1);
    //设置数据位数
    tem_serial->setDataBits(QSerialPort::Data8);
     //设置奇偶校验
    tem_serial->setParity(QSerialPort::NoParity);
    //设置停止位
    tem_serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    tem_serial->setFlowControl(QSerialPort::NoFlowControl);

    return ;
}

void Widget::image_timeout(void)
{
    if(image_tem_is_start==true&&tem_read==false)
    {
        this->tem_time->start(1200);
        tem_read=true;
    }
   this->update();
   return;
}

bool Widget::camera_init()
{
    /*设置为0系统摄像头，设置为1usb摄像头*/
        this->cap=0;
        capture = cvCaptureFromCAM(0);
        if (!cap.isOpened()) // 检查打开是否成功
            return false;
        else
            return true;
}


/*对图片进行写字*/
void Widget::write_image(QImage &image,QString s1,QString s2)
{
    QPainter pp(&image);
    QPen pen = QPen(Qt::red,5);
    QBrush brush = QBrush(Qt::red);
    pp.setPen(pen);
    pp.setBrush(brush);
    pp.drawText(QPointF(20,30),QStringLiteral("测量温度:%1").arg(s1));
    pp.drawText(QPointF(20,50),QStringLiteral("口罩验证成功：%1").arg(s2));
    return;
}
/*重绘事件进行图片每一帧的显示*/
void Widget::paintEvent(QPaintEvent *event)
{
    /*采取每一帧图像*/
    cap >> frame;
    if(image_tem_is_start==true)
    {
        if(process_image(frame)==true)//判断是否戴口罩
        {
            write_image(*image,"未识别","是");
            if(tem_is_ok==true)
            {
                write_image(*image,tem,"是");
                QDateTime current_date_time =QDateTime::currentDateTime();
                this->msg.msg2=current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                this->static_image=*image;
                this->is_use_static_image=true;
                /*打开门*/
                this->open_door();
            }
        }
    }

    (*image)=cvMat_to_Image(frame);
    // 图像不为空，则开始显示
    if(is_use_static_image==true)
    {
        QPainter painter(this);
        QRect frameRect=this->rect();

        /*将界面矩形赋值给对象*/
        frameRect.adjust(2,2,-2,-2);

        //画背景和边框
        painter.setPen(QColor(0,0,0));
        painter.setBrush(QColor(255,255,255));
        painter.drawRoundedRect(frameRect,2,2);
        painter.drawImage(frameRect,static_image);
    }
    else
    {
        QPainter painter(this);
        QRect frameRect=this->rect();

        /*将界面矩形赋值给对象*/
        frameRect.adjust(2,2,-2,-2);

        //画背景和边框
        painter.setPen(QColor(0,0,0));
        painter.setBrush(QColor(255,255,255));
        painter.drawRoundedRect(frameRect,2,2);
        painter.drawImage(frameRect,*image);
    }
}


/*进行图片处理*/
bool Widget::process_image(Mat &mat)
{

    return true;
}

//把QImage转化为cv：：mat
Mat Widget::QImage_to_cvMat(QImage image)
{
    cv::Mat mat;
     switch(image.format())
     {
     case QImage::Format_ARGB32:
     case QImage::Format_RGB32:
     case QImage::Format_ARGB32_Premultiplied:
         mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
         break;
     case QImage::Format_RGB888:
         mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
         cv::cvtColor(mat, mat, CV_BGR2RGB);
         break;
     case QImage::Format_Indexed8:
         mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
         break;
     }
     return mat;
}

//cv::mat转化为QImage
QImage Widget::cvMat_to_Image(const cv::Mat& mat)
{
    if(mat.type() == CV_8UC1)
       {
           QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
           // Set the color table (used to translate colour indexes to qRgb values)
           image.setColorCount(256);
           for(int i = 0; i < 256; i++)
           {
               image.setColor(i, qRgb(i, i, i));
           }
           // Copy input Mat
           uchar *pSrc = mat.data;
           for(int row = 0; row < mat.rows; row ++)
           {
               uchar *pDest = image.scanLine(row);
               memcpy(pDest, pSrc, mat.cols);
               pSrc += mat.step;
           }
           return image;
       }
       // 8-bits unsigned, NO. OF CHANNELS = 3
       else if(mat.type() == CV_8UC3)
       {
           // Copy input Mat
           const uchar *pSrc = (const uchar*)mat.data;
           // Create QImage with same dimensions as input Mat
           QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
           return image.rgbSwapped();
       }
       else if(mat.type() == CV_8UC4)
       {
           qDebug() << "CV_8UC4";
           // Copy input Mat
           const uchar *pSrc = (const uchar*)mat.data;
           // Create QImage with same dimensions as input Mat
           QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
           return image.copy();
       }
       else
       {
           qDebug() << "ERROR: Mat could not be converted to QImage.";
           return QImage();
       }
}
QByteArray Widget::StringToHex(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

char Widget::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
}
Widget::~Widget()
{
    cap.release();
    delete image;
    delete udpsocket;
    delete ui;
}

