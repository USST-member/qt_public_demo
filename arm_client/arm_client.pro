
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

INCLUDEPATH += C:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\include

LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_aruco410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_bgsegm410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_bioinspired410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_calib3d410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_ccalib410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_core410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_datasets410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_dnn410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_dnn_objdetect410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_dpm410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_face410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_features2d410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_flann410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_fuzzy410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_gapi410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_hdf410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_hfs410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_highgui410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_imgcodecs410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_imgproc410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_img_hash410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_line_descriptor410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_ml410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_objdetect410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_optflow410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_phase_unwrapping410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_photo410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_plot410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_quality410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_reg410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_rgbd410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_saliency410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_shape410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_stereo410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_stitching410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_structured_light410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_superres410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_surface_matching410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_text410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_tracking410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_video410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_videoio410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_videostab410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_xfeatures2d410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_ximgproc410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_xobjdetect410
LIBS += -LC:\Users\dongkangjia\Desktop\opencv-4.0.1-mingw\install\x64\mingw\bin -llibopencv_xphoto410

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
