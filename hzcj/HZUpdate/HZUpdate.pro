#-------------------------------------------------
#
# Project created by QtCreator 2018-05-08T15:09:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include($$PWD/../common.pri)

TARGET = HZUpdate
TEMPLATE = app
DESTDIR = $$INSTALL_PATH

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH \
    $$INCLUDE_PATH/HZControls \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/HZDataLayer \
    $$INCLUDE_PATH/ThirdParty \


HEADERS  += \
    $$PWD/updatewindow.h \
    $$PWD/hzupdateuictrl.h \
    $$PWD/updater.h

SOURCES += \
    $$PWD/updatewindow.cpp \
    $$PWD/update_main.cpp \
    $$PWD/hzupdateuictrl.cpp \
    $$PWD/updater.cpp

RESOURCES += \
    $$PWD/qss.qrc

RC_FILE += $$INCLUDE_PATH/hzcj.rc

#dumps
LIBS += -L$$LIB_PATH  -l$$qtLibraryName(quazip) -lHZCore -lHZControls -lHZDataLayer

FORMS += \
    $$PWD/hzendupdate.ui \
    $$PWD/hzstartupdate.ui \
    $$PWD/hzupdateuictrl.ui \
    $$PWD/hzupdating.ui




