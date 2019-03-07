TEMPLATE = subdirs
CONFIG += ordered

include($$PWD/common.pri)

SUBDIRS += \
    HZCore \
    HZDataLayer \
    HzInfoCenter \
    HZFormula \
    HZControls \
    $$MODULES_PATH/keyboard \
    HZAuthentication \
    HZComponent \
    HZUpdate \
    main \
    hzcjd




COMPILER_MODE =
CONFIG(debug, debug|release) {
    COMPILER_MODE = debug
}else{
    COMPILER_MODE = release
}

#$$RUNBASE_DIR/cfg
DEF_FROM_CFG_PATH = $$RUNBASE_DIR/cfg


win32 {
   from_runbase_lib_path = $$RUNBASE_DIR/lib/$${COMPILER_MODE}
   to_lib_path = $$LIB_PATH/
   $$qtDependentCopy($$from_runbase_lib_path, $$to_lib_path)
   from_runbase_inc_path = $$RUNBASE_DIR/include
   to_inc_path = $$INCLUDE_PATH/
   $$qtDependentCopy($$from_runbase_inc_path, $$to_inc_path)
   from_runbase_cfg_path = $$RUNBASE_DIR/cfg
   to_inst_path = $$INSTALL_PATH/
   $$qtDependentCopy($$DEF_FROM_CFG_PATH, $$to_inst_path)
}
