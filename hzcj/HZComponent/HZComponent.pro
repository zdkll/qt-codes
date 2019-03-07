#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T19:32:00
#
#-------------------------------------------------

QT       += widgets webenginewidgets


include($$PWD/../common.pri)

TARGET = HZComponent
TEMPLATE = lib
DESTDIR = $$LIB_PATH
DEFINES += HZCOMPONENT_LIBRARY

INCLUDEPATH += \
    $$PWD  \
    $$PWD/include \
    $$INCLUDE_PATH/HzInfoCenter \
    $$INCLUDE_PATH/HZDataLayer \
    $$INCLUDE_PATH/HZAuthentication \
    $$INCLUDE_PATH/HZControls \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/HZFormula \
    $$INCLUDE_PATH/HZKeyBoard \
    $$INCLUDE_PATH/HZFormula/include



#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

include($$PWD/componet/componet.pri)
include($$PWD/suoleChart/suoleChart.pri)
include($$PWD/login/login.pri)
include($$PWD/chart/chart.pri)
include($$PWD/indicator/indicator.pri)
include($$PWD/kline/kline.pri)
include($$PWD/min/min.pri)
include($$PWD/minimornitor/mornitor.pri)
include($$PWD/marquee/marquee.pri)
include($$PWD/include/include.pri)
include($$PWD/mvc/mvc.pri)
include($$PWD/risefall/risefall.pri)
include($$PWD/information/information.pri)
include($$PWD/smallparts/smallparts.pri)
include($$PWD/quoteLayoutPage/quoteLayoutPage.pri)
include($$PWD/newsview/newsview.pri)
include($$PWD/stockdetailPage/stockdetailPage.pri)
include($$PWD/fundhandicap/fundhandicap.pri)
include($$PWD/handicapcompent/handicapcompent.pri)
include($$PWD/financegraph/finance.pri)
include($$PWD/smartpanel/smartpanel.pri)
include($$PWD/optionalstockpage/optionalstockpage.pri)
include($$PWD/riseFallFlatNum/riseFallFlatNum.pri)
include($$PWD/quotepanel/quotepanel.pri)
include($$PWD/quotelistpage/quotelistpage.pri)
include($$PWD/blocklistpage/blocklistpage.pri)
include($$PWD/infopage/infopage.pri)
include($$PWD/fundchangeview/fundchangeview.pri)
include($$PWD/lhbuysellview/lhbuysellview.pri)
include($$PWD/datacenterpage/datacenterpage.pri)
include($$PWD/biddingChart/biddingChart.pri)
include($$PWD/ftenpage/ftenmarster.pri)
include($$PWD/tickdlg/tickdlg.pri)
include($$PWD/curveGraph/curveGraph.pri)
include($$PWD/StrategyPage/StrategyPage.pri)
include($$PWD/testkline/testkline.pri)
include($$PWD/weekly/weekly.pri)

SOURCES += \



HEADERS += \

LIBS += -L$$LIB_PATH -lHZControls -lHZCore -lHZDataLayer -lHZFormula -lHZAuthentication -lHzInfoCenter
win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}

DISTFILES += \
    quotepanel/quotepanel.pri

