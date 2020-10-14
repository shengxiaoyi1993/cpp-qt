#-------------------------------------------------
#
# Project created by QtCreator 2020-09-28T09:59:27
#
#-------------------------------------------------

QT       += core gui network websockets concurrent

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uvss_device_test_tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -luuid -lssl -lcrypto -lpthread -lz

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 console

SOURCES += \
        ../src/main.cpp \
#        ../src/mainwindow.cpp \
        ../src/toml/src/toml.cpp \
        ../src/cppgc/cppgc.cpp \
        ../src/hex/hex.cpp \
        ../src/cjsonobject/CJsonObject.cpp \
        ../src/cjsonobject/cJSON.cpp \
        ../src/dm_httpobject.cpp \
        ../src/uvsstesttool.cpp \
        ../src/uvss_api.cpp \
    ../src/configrelate.cpp \


HEADERS += \
#        ../src/mainwindow.h \
        ../src/toml/src/toml.h \
        ../src/sha/sha256.h \
        ../src/cppgc/cppgc.h \
        ../src/cppgc/hex.h \
        ../src/cjsonobject/CJsonObject.hpp \
        ../src/cjsonobject/cJSON.h \
        ../src/dm_httpobject.h \
        ../src/uvss_api.h \
        ../src/uvsstesttool.h \
    ../src/configrelate.h \



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../src/restclient-cpp/lib/include

LIBS += -L$$PWD/../src/restclient-cpp/lib/lib -lrestclient-cpp -lcurl
QMAKE_RPATHDIR += $$PWD/../src/restclient-cpp/lib/lib

### 加载当前目录下的 .so 库文件
#unix:!macx: DEPENDPATH += ./
#unix:!macx: QMAKE_RPATHDIR += ./
