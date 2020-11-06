QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../../src/main/main.cpp \
    ../../src/datamanage/datamanage.cpp \
    ../../src/devproxy/devproxy.cpp \
    ../../src/com/comserver.cpp \
    ../../src/schedule/schedule.cpp \
    ../../lib/cjsonobject/cJSON.cpp \
    ../../lib/cjsonobject/CJsonObject.cpp \
    ../../src/com/apiserver.cpp \
    ../../src/ds_def/tcs_ds_def.cpp \
    ../../src/devproxy/basecam.cpp \
    ../../src/controler/controler.cpp



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../src/datamanage/datamanage.h \
    ../../src/devproxy/devproxy.h \
    ../../src/com/comserver.h \
    ../../src/schedule/schedule.h \
    ../../lib/cjsonobject/cJSON.h \
    ../../lib/cjsonobject/CJsonObject.hpp \
    ../../src/com/apiserver.h \
    ../../src/ds_def/tcs_ds_def.h \
    ../../src/devproxy/basecam.h \
    ../../src/controler/controler.h




unix|win32: LIBS += -L$$PWD/../../../common_lib/sskj_lib/com_server/lib/ -lcom_server

INCLUDEPATH += $$PWD/../../../common_lib/sskj_lib/com_server/include
DEPENDPATH += $$PWD/../../../common_lib/sskj_lib/com_server/lib



unix|win32: LIBS += -L$$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/lib/ -lrestclient-cpp

INCLUDEPATH += $$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/include
DEPENDPATH += $$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/lib
