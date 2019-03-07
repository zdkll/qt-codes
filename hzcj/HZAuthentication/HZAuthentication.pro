#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T11:15:03
#
#-------------------------------------------------

QT       -= gui
QT       += core network

include($$PWD/../common.pri)

TARGET = HZAuthentication
TEMPLATE = lib
DESTDIR = $$LIB_PATH
DEFINES += HZAUTHENTICATION_LIBRARY


INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH \
    $$INCLUDE_PATH/ThirdParty \
    $$INCLUDE_PATH/HZCore    \
    $$INCLUDE_PATH/httpclient

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

SOURCES += \
    $$PWD/hzauthenopt.cpp \
    hzopenssl.cpp \
    authencmdexe.cpp \
    localstocksource.cpp \
    zxstockopt.cpp

HEADERS += \
    $$PWD/hzauthenopt.h \
    $$PWD/hzauthentication_global.h \
    hzopenssl.h \
    authencmdexe.h \
    authenpublicdef.h \
    localstocksource.h \
    zxstockopt.h

LIBS += -L$$LIB_PATH -llibeay32 -lssleay32 -lHZCore

win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}



