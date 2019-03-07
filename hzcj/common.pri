!isEmpty(APP_PRI_INCLUDED):error("common.pri already included")
APP_PRI_INCLUDED = 1

APP_VERSION = 1.0.3
APP_COMPAT_VERSION = 1.0.3


#[*find lib function*]
defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

#[*project version test function*]
defineTest(minQtVersion){
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

#[*path setting*]
MAKE_SPEC = $$[QMAKE_SPEC]
LST_TMP = $$split(MAKE_SPEC,-)
contains(LST_TMP,g++){
    MAKE_SPEC = $$first(LST_TMP)-gplusplus
}
PLATFORM_CFG = release/$$[QT_VERSION]/$$MAKE_SPEC
CONFIG(debug,debug|release){
    PLATFORM_CFG = debug/$$[QT_VERSION]/$$MAKE_SPEC
}

#root path
ROOT_PATH       = $$PWD/..

#common 文件夹路径
COMMON_DIR      = $$ROOT_PATH/common

#runbase 文件夹路径
RUNBASE_DIR     = $$ROOT_PATH/runbase

#框架程序路径
FRAME_PATH      = $$ROOT_PATH/shell

#shell 路径
SHELL_PATH      = $$ROOT_PATH/shell

#modules 文件夹路径
MODULES_PATH    = $$ROOT_PATH/modules

#lib 文件夹路径
LIB_PATH = $$COMMON_DIR/lib/$$PLATFORM_CFG
LIB_BASE_PATH   = $$COMMON_DIR/lib/

#pdb 文件夹路径
LIB_PDB_EXPOSE_PATH = $$LIB_BASE_PATH/pdb

#include 文件夹路径
INCLUDE_PATH    = $$COMMON_DIR/include

#examples 文件夹路径
EXAMPLES_PATH   = $$COMMON_DIR/examples

#安装路径
INSTALL_PATH    = $$COMMON_DIR/$$PLATFORM_CFG

#框架内部路径
#CORE_PATH          = $$PWD/cm/$$PLATFORM_CFG
CORE_INCLUDE_PATH   = $$INCLUDE_PATH/core_private
CORE_LIB_PATH       = $$LIB_PATH/core_private

#[*compiler setting*]
win32 {

    DEFINES *= Q_COMPILER_INITIALIZER_LISTS

    #turn off compiler warning
    QMAKE_CXXFLAGS += /wd"4819"

    #add pdb file in release env
    #QMAKE_LFLAGS_RELEASE += /MAP
    #QMAKE_CFLAGS_RELEASE += /Zi
    #QMAKE_LFLAGS_RELEASE += /debug /opt:ref

    #support windows xp
    QMAKE_LFLAGS_WINDOWS= /SUBSYSTEM:WINDOWS,5.01

    #support c++11
    CONFIG += c++11

    #add precompiler header file
    PRECOMPILED_HEADER  = $$INCLUDE_PATH/precompiler_header.h
}

#[*copy files at qmake compile time*]
defineReplace(qtCustomCopy) {
   unset(src_dir)
   unset(dst_dir)

   src_dir = $$1
   dst_dir = $$2

   src_dir ~= s,/,\\,g
   dst_dir ~= s,/,\\,g

   system(xcopy $$src_dir\\*.h* $$dst_dir /y /e /s)
   return ($$1)
}

#[*copy dependent files at qmake compile time*]
defineReplace(qtDependentCopy) {
   unset(src_dir)
   unset(dst_dir)

   src_dir = $$1
   dst_dir = $$2

   src_dir ~= s,/,\\,g
   dst_dir ~= s,/,\\,g

   system(xcopy $$src_dir $$dst_dir /y /e /s)
   return ($$1)
}

defineReplace(stripSrcDir) {
    return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}

!isEmpty(BUILD_TESTS):TEST = 1

isEmpty(TEST):CONFIG(debug, debug|release) {
    !debug_and_release|build_pass {
        TEST = 1
    }
}

INCLUDEPATH += $$PWD

IID_APP_PLUGIN = AppPlugin

!isEmpty(vcproj) {
    DEFINES += IID_APP_PLUGIN=\"$$IID_APP_PLUGIN\"
} else {
    DEFINES += IID_APP_PLUGIN=\\\"$$IID_APP_PLUGIN\\\"
}

DISTFILES += \
    $$PWD/skin/default/qss/3_datacenterpage.qss \
    $$PWD/skin/default/qss/3_strategycenterpage.qss

#内存泄露检测
CONFIG(debug, debug|release) {
    LIBS += -L$$LIB_PATH -lvld
}
