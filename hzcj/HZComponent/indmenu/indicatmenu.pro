#-------------------------------------------------
#
# Project created by QtCreator 2019-02-25T17:33:30
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = indicatmenu
TEMPLATE = app
DEFINES *= Q_COMPILER_INITIALIZER_LISTS

SOURCES += main.cpp\
        indmenu.cpp

HEADERS  += indmenu.h

FORMS    += indmenu.ui

RESOURCES += \
    img.qrc
