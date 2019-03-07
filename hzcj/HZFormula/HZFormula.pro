#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T10:11:03
#
#-------------------------------------------------

QT       -= gui
QT       += widgets

include($$PWD/../common.pri)

TARGET = HZFormula
TEMPLATE = lib
DESTDIR = $$LIB_PATH
DEFINES += HZFORMULA_LIBRARY

INCLUDEPATH += \
    $$PWD \
    $$PWD/include \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/HZDataLayer

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

SOURCES += \
    $$PWD/hzformula.cpp \
    $$PWD/block.cpp \
    $$PWD/CalcCenter.cpp \
    $$PWD/Compiler.cpp \
    $$PWD/Data.cpp \
    $$PWD/Environment.cpp \
    $$PWD/EnvironmentGroup.cpp \
    $$PWD/IRCodeCache.cpp \
    $$PWD/math.cpp \
    $$PWD/memblock.cpp \
    $$PWD/Runtime.cpp

HEADERS += \
    $$PWD/hzformula.h \
    $$PWD/include/CalcCenter.h \
    $$PWD/include/hzformula_global.h \
    $$PWD/include/IndiInterface.h \
    $$PWD/include/memblock.h \
    $$PWD/include/formula_api.h \
    $$PWD/include/IndiArith.hpp \
    $$PWD/Compiler.h \
    $$PWD/Environment.h \
    $$PWD/EnvironmentGroup.h \
    $$PWD/hzformula_global.h \
    $$PWD/IRCodeCache.h \
    $$PWD/jcformuladef.h \
    $$PWD/Runtime.h

LIBS += -L$$LIB_PATH -lHZCore -lHZDataLayer -llua51

win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}
