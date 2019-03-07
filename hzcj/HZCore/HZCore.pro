#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T09:33:51
#
#-------------------------------------------------

QT       -= gui
QT       += core widgets  network

include($$PWD/../common.pri)
include($$PWD/log/log.pri)
include($$PWD/httpclient/httpclient.pri)

TARGET = HZCore
TEMPLATE = lib
DESTDIR = $$LIB_PATH
DEFINES += HZCORE_LIBRARY
DEFINES += HZLOG_BULID_LIB
DEFINES += HZ_AUTO_ELAPSED_TIMER_ENABAL

INCLUDEPATH += \
    $$PWD \
    $$PWD/include \
    $$INCLUDE_PATH/HZCore \
    $$PWD/httpclient \
    $$INCLUDE_PATH/ThirdParty

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

HEADERS +=\
    $$PWD/include/hzcore_global.h \
    $$PWD/include/Any.hpp \
    $$PWD/include/buffer.h \
    $$PWD/include/cfgdef.hpp \
    $$PWD/include/hzcfg.h \
    $$PWD/include/hzcore_global.h \
    $$PWD/include/Ioc.hpp \
    $$PWD/include/micro.h \
    $$PWD/include/NonCopyable.hpp \
    $$PWD/include/Observer.hpp \
    $$PWD/include/StyleSheet.h \
    $$PWD/include/SyncQueue.hpp \
    $$PWD/include/ThreadPool.hpp \
    $$PWD/include/urlparse.h \
    $$PWD/include/downloadupdate.h \
    $$PWD/include/hzcjinfo.h \
    $$PWD/include/verinfoparse.h \
    $$PWD/include/hzuserdatamgr.h \
    $$PWD/include/hzdirhelper.h \
    $$PWD/include/HChineseConvertToPY.h \
    include/hzpublic.hpp

SOURCES += \
    $$PWD/urlparse.cpp \
    $$PWD/ThreadPool.cpp \
    $$PWD/hzcfg.cpp \
    $$PWD/StyleSheet.cpp \
    $$PWD/verinfoparse.cpp \
    $$PWD/downloadupdate.cpp \
    $$PWD/hzuserdatamgr.cpp \
    $$PWD/hzdirhelper.cpp \
    $$PWD/include/HChineseConvertToPY.cpp

LIBS += -L$$LIB_PATH -l$$qtLibraryName(log4cpp)

win32 {
    frome_src_inc_path = $$PWD/include
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_inc_path, $$to_dest_path)
    frome_src_path = $$PWD/log
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
    frome_src_path = $$PWD/httpclient
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}
