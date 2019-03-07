#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T09:37:37
#
#-------------------------------------------------

QT       += widgets webenginewidgets

include($$PWD/../common.pri)

TARGET = HZControls
TEMPLATE = lib
DEFINES += HZCONTROLS_LIBRARY
DESTDIR = $$LIB_PATH

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH/HZCore

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

SOURCES += \
    $$PWD/hzcontrols.cpp \
    $$PWD/basewidget.cpp \
    $$PWD/basepage.cpp \
    $$PWD/popupwindow.cpp \
    $$PWD/windowcontrolbox.cpp \
    $$PWD/hzobject.cpp \
    $$PWD/subtab.cpp \
    $$PWD/BaseMainWidget.cpp \
    $$PWD/titlebar.cpp \
    $$PWD/frameless_helper.cpp \
    $$PWD/framewidget.cpp \
    $$PWD/treeview/hzitemdelegate.cpp \
    $$PWD/treeview/hzitemmodel.cpp \
    $$PWD/treeview/hztreeview.cpp \
    $$PWD/treeview/treenode.cpp \
    $$PWD/basewebview.cpp \
    $$PWD/hzline.cpp \
    $$PWD/sidebar.cpp \
    $$PWD/statusbar.cpp \
    $$PWD/navigatorbar.cpp \
    $$PWD/mesbox.cpp \
    $$PWD/popupdialog.cpp \
    $$PWD/crossinfodlg.cpp \
    $$PWD/freezetableview.cpp \
    $$PWD/parasettingdialog.cpp \
    $$PWD/frozentableview.cpp \
    $$PWD/activitylabel.cpp \
    $$PWD/introductionbutton.cpp \
    $$PWD/scrollwidget.cpp \
    $$PWD/basestackwidget.cpp \
    $$PWD/suoluewidget.cpp \
    $$PWD/hzpixlayer.cpp \
    viewtipspanel.cpp \
    drawlinetextpanel.cpp

HEADERS += \
    $$PWD/hzcontrols.h \
    $$PWD/hzcontrols_global.h \
    $$PWD/basewidget.h \
    $$PWD/basepage.h \
    $$PWD/popupwindow.h \
    $$PWD/windowcontrolbox.h \
    $$PWD/hzobject.h \
    $$PWD/subtab.h \
    $$PWD/BaseMainWidget.h \
    $$PWD/titlebar.h \
    $$PWD/frameless_helper.h \
    $$PWD/framewidget.h \
    $$PWD/treeview/hzitemdelegate.h \
    $$PWD/treeview/hzitemmodel.h \
    $$PWD/treeview/hztreeview.h \
    $$PWD/treeview/treenode.h \
    $$PWD/basewebview.h \
    $$PWD/hzline.h \
    $$PWD/sidebar.h \
    $$PWD/statusbar.h \
    $$PWD/navigatorbar.h \
    $$PWD/mesbox.h \
    $$PWD/popupdialog.h \
    $$PWD/crossinfodlg.h \
    $$PWD/freezetableview.h \
    $$PWD/parasettingdialog.h \
    $$PWD/frozentableview.h \
    $$PWD/activitylabel.h \
    $$PWD/introductionbutton.h \
    $$PWD/scrollwidget.h \
    $$PWD/basestackwidget.h \
    $$PWD/suoluewidget.h \
    $$PWD/hzpixlayer.h \
    viewtipspanel.h \
    drawlinetextpanel.h

FORMS += \
    $$PWD/windowcontrolbox.ui \
    $$PWD/titlebar.ui \
    $$PWD/hzpixlayer.ui

LIBS += -L$$LIB_PATH -lHZCore

win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}






