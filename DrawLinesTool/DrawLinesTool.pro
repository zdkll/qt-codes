#-------------------------------------------------
#
# Project created by QtCreator 2019-02-26T16:59:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrawLinesTool
TEMPLATE = app

CONFIG += c++11

DEFINES += Q_COMPILER_INITIALIZER_LISTS

DESTDIR = $$PWD/bin


SOURCES += main.cpp\
    controls.cpp \
        widget.cpp \
    linesdrawer.cpp \
    drawlinepanel.cpp \
    line.cpp \
    poscalculator.cpp \
    linealgorith.cpp \
    linesettingdialogs.cpp


HEADERS  += widget.h \
       controls.h \
    linesdrawer.h \
    drawlinepanel.h \
    line.h \
    drawlinepublic.h \
    poscalculator.h \
    kpoint.h \
    Any.hpp \
    Ioc.hpp \
    NonCopyable.hpp \
    linealgorith.h \
    linesettingdialogs.h


FORMS    += widget.ui

RESOURCES += \
    qrc.qrc

