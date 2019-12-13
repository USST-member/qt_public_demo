#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<QCamera>
#include<QVideoWidget>
#include<QCameraImageCapture>

class mywidget : public QWidget
{
    Q_OBJECT
public:
    explicit mywidget(QWidget *parent = nullptr);

signals:

public slots:
public:
    QCamera *mCam;
    QVideoWidget *widget;
    QCameraImageCapture *icapture;
};

#endif // MYWIDGET_H
