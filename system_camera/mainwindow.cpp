#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mywidget.h"
#include<QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include<QCameraImageCapture>
#include <QtSerialBus>
#include <QCanBus>
#include<QCanBusDeviceInfo>
#include<QFileDialog>
#include<QMessageBox>
#pragma execution_character_set("utf-8")
/*
*QSerialPort：提供访问串口的功能
*QSerialPortInfo：提供系统中存在的串口的信息
* 属于RS232
*/
#define  BaudRate 115200

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("划片");
    resize(1000,600);


        serial = new QSerialPort;
        //设置串口名
        serial->setPortName("COM3");
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(BaudRate);
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);
         //设置奇偶校验
        serial->setParity(QSerialPort::NoParity);
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);


    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::show_camera);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::close_camera);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::get_image);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
}

bool MainWindow::is_camera_open=false;

void MainWindow::show_camera()
{
    mw.setWindowTitle("ccd");
    mw.resize(200,120);
    mw.widget->setFixedSize(200,120);
    mw.setMaximumSize(200,120);
    mw.setMinimumSize(200,120);
    mw.mCam->start();
    mw.show();
    is_camera_open=true;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::get_image()
{
    /*判断摄像头是否打开，如果打开可以截图
     *否则询问是否打开进行截图
     */
    if(is_camera_open==true)
    {
        QString fileName=QFileDialog::getSaveFileName(this,"保存图片","/");//获取文件的保存路径
        mw.icapture->capture(fileName);
    }
    else
    {
        switch (QMessageBox::question(this,"摄像头未开启","摄像头尚未开启，是否要开启它？",QMessageBox::Ok|QMessageBox::Cancel)) {
        case QMessageBox::Ok:
        {
            show_camera();
            QString fileName=QFileDialog::getSaveFileName(this,"保存图片","/");//获取文件的保存路径
            mw.icapture->capture(fileName);
            break;
        }
        default:
            break;

        }
    }
}
void MainWindow::close_camera()
{
    mw.mCam->stop();
    mw.close();
    is_camera_open=false;
}

void MainWindow::Read_Data()
{
     QByteArray buf;
     buf = serial->readAll();
}



