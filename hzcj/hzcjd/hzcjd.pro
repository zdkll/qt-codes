QT += core

include($$PWD/../common.pri)

CONFIG += c++11

TARGET = shhzcjd

DESTDIR  = $$INSTALL_PATH

CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    start_main.cpp

RC_FILE += $$INCLUDE_PATH/hzcj.rc
