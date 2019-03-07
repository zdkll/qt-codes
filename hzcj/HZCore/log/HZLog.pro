QT += core widgets
CONFIG += c++11

include($$PWD/../common.pri)

TARGET = HZLog
CONFIG += console
CONFIG -= app_bundle
DESTDIR = $$LIB_PATH
TEMPLATE = lib
DEFINES += HZLOG_BULID_LIB

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/ThirdParty

SOURCES += \
    hzlog.cpp
    #main.cpp \

HEADERS += \
    singleton.h \
    hzlogexport.h \
    hzlog.h

LIBS += -L$$LIB_PATH -l$$qtLibraryName(log4cpp) -lHZCore

win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}

#CHM.files += $$HEADERS
#CHM.path = $$INCLUDE_PATH/$${TARGET}
#INSTALLS += CMH

DISTFILES +=
