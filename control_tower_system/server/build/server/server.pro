#-------------------------------------------------
#
# Project created by QtCreator 2020-10-23T19:48:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
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
    ../../../common_component/api_client/apiclient.cpp \
    ../../../common_component/ds_def/tcs_ds_def.cpp \
    ../../../common_lib/third_party_lib/cjsonobject/CJsonObject.cpp \
    ../../../common_lib/third_party_lib/cjsonobject/cJSON.cpp \
        ../../src/main/main.cpp \
        ../../src/main/mainwindow.cpp \
        ../../src/view/userconfig.cpp \
    ../../src/view/loginserver.cpp \
    ../../src/view/useritem.cpp \
    ../../src/view/devconfig.cpp \
    ../../src/view/devitem.cpp \
    ../../src/view/nvrconfig.cpp \
    ../../src/controller/controller.cpp \



HEADERS += \
    ../../../common_component/api_client/apiclient.h \
    ../../../common_component/ds_def/tcs_ds_def.h \
    ../../../common_lib/third_party_lib/cjsonobject/CJsonObject.hpp \
    ../../../common_lib/third_party_lib/cjsonobject/cJSON.h \
        ../../src/main/mainwindow.h \
        ../../src/view/userconfig.h \
    ../../src/view/loginserver.h \
    ../../src/view/useritem.h \
    ../../src/view/devconfig.h \
    ../../src/view/devitem.h \
    ../../src/view/nvrconfig.h \
    ../../src/controller/controller.h \

FORMS += \
        ../../src/view/mainwindow.ui \
        ../../src/view/userconfig.ui \
    ../../src/view/loginserver.ui \
    ../../src/view/useritem.ui \
    ../../src/view/devconfig.ui \
    ../../src/view/devitem.ui \
    ../../src/view/nvrconfig.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

INCLUDEPATH += $$PWD/../../../common_component/
INCLUDEPATH += $$PWD/../../../common_lib/third_party_lib/


win32: LIBS += -L$$PWD/../../../common_lib/sskj_lib/com_client/lib/dll/ -llibcom.dll
unix: LIBS += -L$$PWD/../../../common_lib/sskj_lib/com_client/lib/so/ -lcom

INCLUDEPATH += $$PWD/../../../common_lib/sskj_lib/com_client/include
DEPENDPATH += $$PWD/../../../common_lib/sskj_lib/com_client/include


unix:!macx|win32: LIBS += -L$$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/lib/ -lrestclient-cpp

INCLUDEPATH += $$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/include
DEPENDPATH += $$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/include
