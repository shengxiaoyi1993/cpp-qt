QT -= gui
QT += serialport
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
    ../../../common_component/api_server/apiserver.cpp \
    ../../../common_component/comcpp_server/comserver.cpp \
    ../../../common_component/ds_def/tcs_ds_def.cpp \
    ../../../common_component/syslogger/syslogger.cpp \
    ../../../common_lib/third_party_lib/cjsonobject/CJsonObject.cpp \
    ../../../common_lib/third_party_lib/cjsonobject/cJSON.cpp \
    ../../src/devproxy/uartconvert.cpp \
        ../../src/main/main.cpp \
    ../../src/datamanage/datamanage.cpp \
    ../../src/devproxy/devproxy.cpp \
    ../../src/schedule/schedule.cpp \
    ../../src/devproxy/basecam.cpp \
    ../../src/controler/controler.cpp \
    ../../src/devproxy/nvrdev.cpp



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RC_FILE += server_service.rc
rc_file += server_service.gif

HEADERS += \
    ../../../common_component/api_server/apiserver.h \
    ../../../common_component/comcpp_server/comserver.h \
    ../../../common_component/syslogger/syslogger.h \
    ../../../common_lib/third_party_lib/cjsonobject/CJsonObject.hpp \
    ../../../common_lib/third_party_lib/cjsonobject/cJSON.h \
    ../../../common_component/ds_def/tcs_ds_def.h \
    ../../src/datamanage/datamanage.h \
    ../../src/devproxy/devproxy.h \
    ../../src/devproxy/directcam_reg_def.h \
    ../../src/devproxy/uartconvert.h \
    ../../src/schedule/schedule.h \
    ../../src/devproxy/basecam.h \
    ../../src/controler/controler.h \
    ../../src/devproxy/nvrdev.h


INCLUDEPATH += $$PWD/../../../common_component/
INCLUDEPATH += $$PWD/../../../common_lib/third_party_lib/


unix:!macx: LIBS += -L$$PWD/../../../common_lib/sskj_lib/com_server/lib/so/ -lcom_server
win32: LIBS += -L$$PWD/../../../common_lib/sskj_lib/com_server/lib/dll/ -llibcom_server.dll


INCLUDEPATH += $$PWD/../../../common_lib/sskj_lib/com_server/include
DEPENDPATH += $$PWD/../../../common_lib/sskj_lib/com_server/lib


unix: LIBS += -L$$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/lib/ -lrestclient-cpp
win32: LIBS += -L$$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/lib/ -lrestclient-cpp


INCLUDEPATH += $$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/include
DEPENDPATH += $$PWD/../../../common_lib/third_party_lib/restclient-cpp/lib/include
