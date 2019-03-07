#-------------------------------------------------
#
# Project created by QtCreator 2018-08-29T15:39:23
#
#-------------------------------------------------

QT       -= gui
QT       += core widgets  network


include($$PWD/../common.pri)

TARGET = HzInfoCenter
TEMPLATE = lib
DESTDIR = $$LIB_PATH
DEFINES += HZINFOCENTER_LIBRARY

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/HZDataLayer

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO


SOURCES += $$PWD/hzinfocenter.cpp \
           $$PWD/infojsonparser.cpp

HEADERS += $$PWD/hzinfocenter.h\
           $$PWD/hzinfocenter_global.h \
           $$PWD/hzinfodata.h \
           $$PWD/infojsonparser.h

LIBS += -L$$LIB_PATH -lHZCore -lHZDataLayer

win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}
