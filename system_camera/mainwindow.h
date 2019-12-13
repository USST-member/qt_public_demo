#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<mywidget.h>
#include<QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include <QtSerialBus>
#include <QCanBus>
#include<QCanBusDeviceInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void show_camera();
    void close_camera();
    void get_image();
    void Read_Data();

private:
    Ui::MainWindow *ui;

public:
    mywidget mw;
    QSerialPort *serial ;

    /*用于判断摄像头是否打开*/
    static bool is_camera_open;
};

#endif // MAINWINDOW_H
