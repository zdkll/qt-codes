#-------------------------------------------------
#
# Project created by QtCreator 2019-02-25T11:20:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PageHelper
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    pages.cpp \
    pagehelper.cpp \
    ioccontainter.cpp

HEADERS  += widget.h \
    publicdef.h \
    pages.h \
    pagehelper.h \
    ioccontainter.h \
    noncopyable.h \
    any.h

FORMS    += widget.ui
