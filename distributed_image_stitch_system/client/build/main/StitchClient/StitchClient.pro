#-------------------------------------------------
#
# Project created by QtCreator 2021-01-08T09:29:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StitchClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    ../../../src/view/mainwindow.cpp \
    ../../../src/main/main.cpp \
    ../../../src/controller/controller.cpp \
    ../../../../common_lib/sskj_lib/threadpool/threadpool.cpp \
    ../../../../common_lib/sskj_lib/threadpool/taskexecutor.cpp \
    ../../../../common_lib/sskj_lib/threadpool/concretetaskexecutor.cpp \
    ../../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.cpp \
    ../../../../common_lib/sskj_lib/api_diss/dissapi.cpp \
    ../../../src/view/initview.cpp


HEADERS += \
    ../../../src/view/mainwindow.h \
    ../../../src/controller/controller.h \
    ../../../../common_lib/sskj_lib/threadpool/threadpool.h \
    ../../../../common_lib/sskj_lib/threadpool/taskexecutor.h \
    ../../../../common_lib/sskj_lib/threadpool/concretetaskexecutor.h \
    ../../../../common_lib/sskj_lib/ds_df_diss/ds_df_diss.h \
    ../../../../common_lib/sskj_lib/api_diss/dissapi.h \
    ../../../src/view/initview.h

FORMS += \
    ../../../src/view/mainwindow.ui \
    ../../../src/view/initview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix|win32: LIBS += -L$$PWD/../../../../common_lib/sskj_lib/com_client/lib/so/ -lcom

INCLUDEPATH += $$PWD/../../../../common_lib/sskj_lib/com_client/include
DEPENDPATH += $$PWD/../../../../common_lib/sskj_lib/com_client/include



unix|win32: LIBS += -L$$PWD/../../../../common_lib/sskj_lib/com_server/lib/so/ -lcom_server

INCLUDEPATH += $$PWD/../../../../common_lib/sskj_lib/com_server/include
DEPENDPATH += $$PWD/../../../../common_lib/sskj_lib/com_server/include



unix|win32: LIBS += -L$$PWD/../../../../common_lib/third_party_lib/restclient-cpp/lib/lib/ -lrestclient-cpp

INCLUDEPATH += $$PWD/../../../../common_lib/third_party_lib/restclient-cpp/lib/include
DEPENDPATH += $$PWD/../../../../common_lib/third_party_lib/restclient-cpp/lib/include


unix|win32: LIBS += -L$$PWD/../../../../common_lib/sskj_lib/runway_scan_stitch/lib/linux/ -lrunway_scan_stitch_lib

INCLUDEPATH += $$PWD/../../../../common_lib/sskj_lib/runway_scan_stitch/include
DEPENDPATH += $$PWD/../../../../common_lib/sskj_lib/runway_scan_stitch/include



unix|win32: LIBS += -L$$PWD/../../../../common_lib/third_party_lib/opencv-3.4.1/lib/lib64/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

INCLUDEPATH += $$PWD/../../../../common_lib/third_party_lib/opencv-3.4.1/lib/include
DEPENDPATH += $$PWD/../../../../common_lib/third_party_lib/opencv-3.4.1/lib/include
