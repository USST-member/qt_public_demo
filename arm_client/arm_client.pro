
QT  +=  multimedia multimediawidgets  #摄像头模块
QT       += serialport #串口模块
QT +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

INCLUDEPATH += D:\OpenCV-MinGW-Build\include

LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_calib3d341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_calib3d341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_core341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_dnn341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_features2d341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_flann341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_highgui341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_imgcodecs341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_imgproc341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_ml341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_objdetect341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_photo341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_shape341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_stitching341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_superres341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_video341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_videoio341
LIBS += -LD:\OpenCV-MinGW-Build\x64\mingw\bin -llibopencv_videostab341

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
