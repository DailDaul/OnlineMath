QT = core
QT -= gui
QT += network
QT += sql

CONFIG += c++11 console
COMFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        functionsforserver.cpp \
        main.cpp \
        mytcpserver.cpp

TRANSLATIONS += \
    echoSserver_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    dataBase.h \
    functionsforserver.h \
    mytcpserver.h
