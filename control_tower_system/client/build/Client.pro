#-------------------------------------------------
#
# Project created by QtCreator 2020-10-24T15:17:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
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
        ../../../src/main/main.cpp \
        ../../../src/main/mainwindow.cpp \
    ../src/view/loginclient.cpp \
    ../src/view/toaircamview.cpp \
    ../src/view/togroudcamview.cpp \
    ../src/view/directcamview.cpp \
    ../src/view/electronicsituationmap.cpp

HEADERS += \
        ../../../src/main/mainwindow.h \
    ../src/view/loginclient.h \
    ../src/view/toaircamview.h \
    ../src/view/togroudcamview.h \
    ../src/view/directcamview.h \
    ../src/view/electronicsituationmap.h

FORMS += \
        ../../../src/view/mainwindow.ui \
    ../src/view/loginclient.ui \
    ../src/view/toaircamview.ui \
    ../src/view/togroudcamview.ui \
    ../src/view/directcamview.ui \
    ../src/view/electronicsituationmap.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
