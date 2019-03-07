QT += core gui widgets


CONFIG += console

TARGET = testlib

include($$PWD/../common.pri)

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH/HZLog \
    $$INCLUDE_PATH/ThirdParty \
    $$INCLUDE_PATH/HZCore

LIBS += -L$$LIB_PATH -lHZLog -lHZCore

SOURCES += main.cpp

