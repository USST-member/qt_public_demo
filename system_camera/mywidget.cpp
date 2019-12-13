#include "mywidget.h"
#include <QWidget>
#include<QCamera>
#include<QVideoWidget>
#include<QCameraImageCapture>

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{

    mCam=new QCamera(this);
    widget=new QVideoWidget(this);

    icapture=new QCameraImageCapture(mCam);
    icapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

    /*mCam->setCaptureMode(QCamera::CaptureStillImage);
     *和下面这句话效果一样
     */
    mCam->setCaptureMode(QCamera::CaptureVideo);
    mCam->setViewfinder(widget);

}
