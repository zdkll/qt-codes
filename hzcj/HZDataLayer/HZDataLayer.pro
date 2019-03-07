#-------------------------------------------------
#
# Project created by QtCreator 2018-04-23T13:58:50
#
#-------------------------------------------------
QT += core widgets network

include($$PWD/../common.pri)

TARGET = HZDataLayer
TEMPLATE = lib
DESTDIR = $$LIB_PATH
DEFINES += HZDATALAYER_LIBRARY

INCLUDEPATH += \
    $$PWD \
    $$INCLUDE_PATH \
    $$INCLUDE_PATH/HZCore \
    $$INCLUDE_PATH/ThirdParty \
    $$PWD/proto \
    $$PWD/clientsocket

#relase env debug
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

include($$PWD/proto/proto.pri)
include($$PWD/clientsocket/clientsocket.pri)

SOURCES += \
    $$PWD/datacontroller.cpp \
    $$PWD/packetprocess.cpp \ 
    $$PWD/httpdata.cpp \
    $$PWD/datakline.cpp \
    $$PWD/datasimulator.cpp \
    $$PWD/quotepacket.cpp \
    $$PWD/requestsender.cpp \
    $$PWD/responsereciever.cpp

HEADERS += \
    $$PWD/datamodeldef.h\
    $$PWD/hzdatalayer_global.h \
    $$PWD/datacontroller.h \
    $$PWD/packetprocess.h \
    $$PWD/protobufs.h \
    $$PWD/clienthds.h \
    $$PWD/gwdefine.h \
    $$PWD/httpdata.h \
    $$PWD/datactrldef.h \
    $$PWD/datamodeldefs/datamodelpublic.h \
    $$PWD/datamodeldefs/stockinfodef.h \
    $$PWD/datamodeldefs/klinedef.h \
    $$PWD/datamodeldefs/tickdef.h \
    $$PWD/datamodeldefs/funddef.h \
    $$PWD/datamodeldefs/baseinfodef.h \
    $$PWD/datamodeldefs/publishdef.h \
    $$PWD/datakline.h \
    $$PWD/datasimulator.h \
    $$PWD/quotepacket.h \
    $$PWD/requestsender.h \
    $$PWD/responsereciever.h \
    datamodeldefs/fundabnormaldef.h \
    datamodeldefs/biddingdef.h \
    datamodeldefs/risefallmonitordef.h \
    datamodeldefs/shotlinemotiondef.h \
    datamodeldefs/broadcast.h \
    datamodeldefs/zhibiao.h

LIBS += -L$$LIB_PATH -llibprotobuf -llibeay32 -lssleay32 -lHZCore

win32 {
    frome_src_path = $$PWD
    to_dest_path = $$INCLUDE_PATH/$${TARGET}/
    $$qtCustomCopy($$frome_src_path, $$to_dest_path)
}

DISTFILES += \
    msg_mask_bit_desc.txt \
    cmd_file.txt


