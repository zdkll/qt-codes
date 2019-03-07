#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T09:36:33
#
#-------------------------------------------------

QT       += core gui webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/../common.pri)
include($$PWD/maincompent/maincomponent.pri)
#include($$MODULES_PATH/)

TEMPLATE = app
TARGET = shhzcjEx
DESTDIR  = $$INSTALL_PATH
DEFINES += AUTO_ELAPSED_TIMER_ENABAL

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH \
    $$INCLUDE_PATH/HZLog \
    $$INCLUDE_PATH/HZControls \
    $$INCLUDE_PATH/HZDataLayer \
    $$INCLUDE_PATH/HZAuthentication \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/ThirdParty \
    $$INCLUDE_PATH/ThirdParty/breakpad \
    $$INCLUDE_PATH/HZComponent/include \
    $$INCLUDE_PATH/HZComponent/minimornitor \
    $$INCLUDE_PATH/HZComponent \
    $$INCLUDE_PATH/HZKeyBoard    \
    $$INCLUDE_PATH/HZComponent/smallparts \
    $$INCLUDE_PATH/HZComponent/newsview \
    $$INCLUDE_PATH/HZComponent/tickdlg \
    $$INCLUDE_PATH/HZComponent/optionalstockpage

#zdk test
INCLUDEPATH += \
  $$INCLUDE_PATH/HZComponent/mvc/itemdelegates \
 $$INCLUDE_PATH/HZComponent/mvc/itemmodels \
 $$INCLUDE_PATH/HZComponent/mvc/itemviews \
 $$INCLUDE_PATH/HZComponent/mvc/dataprovider \
 $$INCLUDE_PATH/HZComponent/mvc \
 $$INCLUDE_PATH/HZComponent/fundhandicap \
 $$INCLUDE_PATH/HzInfoCenter \

HEADERS  += \
    $$PWD/prepareupdate.h \
    $$PWD/hzapplication.h \
    $$PWD/dumpprocesser.h \
    $$PWD/hzmainwnd.h \
    $$PWD/hzpagehelper.h \
    $$PWD/test.h \
    $$PWD/shortcut.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/prepareupdate.cpp \
    $$PWD/hzapplication.cpp \
    $$PWD/dumpprocesser.cpp \
    $$PWD/hzmainwnd.cpp \
    $$PWD/hzpagehelper.cpp \
    $$PWD/test.cpp \
    $$PWD/shortcut.cpp


RESOURCES += \
    $$PWD/resource.qrc

RC_FILE += $$INCLUDE_PATH/hzcj.rc

LIBS += -L$$LIB_PATH -lHZCore -lHZControls -lHZDataLayer -lHZComponent -lHZAuthentication -lHZKeyBoard -lHzInfoCenter
LIBS += -L$$LIB_PATH -ldbghelp -lcommon -lcrash_generation_client -lexception_handler

#[*pdb file install*]
INSTALL_PDB_PATH.files += $$LIB_PATH/*.pdb
INSTALL_PDB_PATH.path = $$INSTALL_PATH/PDB

INSTALL_PDB_PATH_EX.files += $${LIB_PATH}/*.pdb
INSTALL_PDB_PATH_EX.path = $${INSTALL_PATH}/PDB

EXE_PDB_INSTALL.files += $$INSTALL_PATH/*.pdb
EXE_PDB_INSTALL.path = $$INSTALL_PATH/PDB

EXE_PDB_INSTALL_EX.files += $${INSTALL_PATH}/*.pdb
EXE_PDB_INSTALL_EX.path = $${INSTALL_PATH}/PDB

#[*lib file install run env*]
INSTALL_LIB_PATH.files += $$LIB_PATH/*.dll
INSTALL_LIB_PATH.path = $$INSTALL_PATH

INSTALL_LIB_PATH_EX.files += $${LIB_PATH}/*.dll
INSTALL_LIB_PATH_EX.path = $${INSTALL_PATH}

INSTALLS += INSTALL_PDB_PATH INSTALL_PDB_PATH_EX INSTALL_LIB_PATH INSTALL_LIB_PATH_EX EXE_PDB_INSTALL EXE_PDB_INSTALL_EX

FORMS += \
    $$PWD/hzstartupdatemain.ui



