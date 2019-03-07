INCLUDEPATH += $$PWD
include($$PWD/hotinfo/hotinfo.pri)
include($$PWD/annc/annc.pri)
include($$PWD/news/news.pri)
include($$PWD/calendar/calendar.pri)
include($$PWD/multihour/multihour.pri)

DEFINES += BULID_LIB

HEADERS += \
    $$PWD/infomaster.h \
    $$PWD/infodef.h \
    $$PWD/indexreqbutton.h \
    $$PWD/infostyle.h \
    $$PWD/calandarbar.h \
    $$PWD/updatinterface.h \
    $$PWD/infodataprovidor.h \
    $$PWD/infoitemtitle.h \
    $$PWD/infopage_export.h \
    $$PWD/infowebview.h \
    $$PWD/webpopupwindow.h \
    $$PWD/hzcalandarctrl.h

SOURCES +=  \
    $$PWD/infomaster.cpp \
    $$PWD/indexreqbutton.cpp \
    $$PWD/infostyle.cpp \
    $$PWD/calandarbar.cpp \
    $$PWD/updatinterface.cpp \
    $$PWD/infodataprovidor.cpp \
    $$PWD/infoitemtitle.cpp \
    $$PWD/infowebview.cpp \
    $$PWD/webpopupwindow.cpp \
    $$PWD/hzcalandarctrl.cpp

FORMS += \
    $$PWD/certificateerrordialog.ui \
    $$PWD/passworddialog.ui \
    $$PWD/hzcalandarctrl.ui


