INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/ClientSocket.cpp \
    $$PWD/packctrl.cpp \
    $$PWD/rsasignature.cpp

HEADERS += \
    $$PWD/ClientSocket.h \
    $$PWD/head.h \
    $$PWD/packctrl.h \
    $$PWD/rsasignature.h \

CLIENT_SOCKET_HEADER.files = $${HEADERS}
