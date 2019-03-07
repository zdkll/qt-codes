INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/base_news.pb.cc \
    $$PWD/bidding.pb.cc \
    $$PWD/inner_svr.pb.cc \
    $$PWD/kline.pb.cc \
    $$PWD/public_message.pb.cc \
    $$PWD/publish.pb.cc \
    $$PWD/rank.pb.cc \
    $$PWD/ret_base.pb.cc \
    $$PWD/search.pb.cc \
    $$PWD/stock_basinfo.pb.cc \
    $$PWD/stock_code.pb.cc \
    $$PWD/user.pb.cc \
    $$PWD/zhubi.pb.cc \
    $$PWD/rise_fall_monitor.pb.cc \
    $$PWD/short_motion.pb.cc \
    $$PWD/broadcast.pb.cc \
    $$PWD/zhibiao.pb.cc \
    $$PWD/mid_abnormal.pb.cc


HEADERS += \
    $$PWD/base_news.pb.h \
    $$PWD/bidding.pb.h \
    $$PWD/inner_svr.pb.h \
    $$PWD/kline.pb.h \
    $$PWD/public_message.pb.h \
    $$PWD/publish.pb.h \
    $$PWD/rank.pb.h \
    $$PWD/ret_base.pb.h \
    $$PWD/search.pb.h \
    $$PWD/stock_basinfo.pb.h \
    $$PWD/stock_code.pb.h \
    $$PWD/user.pb.h \
    $$PWD/zhubi.pb.h \
    $$PWD/rise_fall_monitor.pb.h \
    $$PWD/short_motion.pb.h \
    $$PWD/broadcast.pb.h \
    $$PWD/zhibiao.pb.h \
    $$PWD/mid_abnormal.pb.h


PROTO_HEADER.files = $${HEADERS}
