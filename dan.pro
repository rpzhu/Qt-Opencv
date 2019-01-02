#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T15:30:52
#
#-------------------------------------------------

QT       += core gui
QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dan
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    autowork.cpp \
    setting.cpp \
    shoudongceshi.cpp \
    myframe.cpp \
    chuangjian.cpp \
    logindialog.cpp \
    camaraget.cpp \
    changepassword.cpp

HEADERS  += mainwindow.h \
    autowork.h \
    setting.h \
    shoudongceshi.h \
    myframe.h \
    chuangjian.h \
    logindialog.h \
    camaraget.h \
    changepassword.h

FORMS    += mainwindow.ui \
    autowork.ui \
    setting.ui \
    shoudongceshi.ui \
    chuangjian.ui \
    logindialog.ui \
    changepassword.ui

RESOURCES += \
    res.qrc


INCLUDEPATH += C:\opencv\MinGw_build\install\include
LIBS += -LC:\opencv\MinGw_build\install\x86\mingw\bin\
    libopencv_core2413 \
    libopencv_highgui2413 \
    libopencv_imgproc2413 \
    libopencv_features2d2413 \
    libopencv_calib3d2413 \
