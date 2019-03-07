#-------------------------------------------------
#
# Project created by QtCreator 2018-09-19T08:57:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = axist
TEMPLATE = app

include($PWD/../../../common.pri)

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/HzInfoCenter

LIBS += -L$$LIB_PATH -lHZCore -lHzInfoCenter

SOURCES += main.cpp\
    hzfinancestyleinfo.cpp \
    financemarster.cpp

HEADERS  += \
    hzfinancestyleinfo.h \
    financemarster.h

FORMS    += financemarster.ui

RESOURCES += \
    img.qrc
