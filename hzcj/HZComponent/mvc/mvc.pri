INCLUDEPATH += $$PWD

include($$PWD/itemmodels/itemmodels.pri)
include($$PWD/dataprovider/dataprovider.pri)
include($$PWD/itemviews/itemviews.pri)
include($$PWD/itemdelegates/itemdelegates.pri)

HEADERS += \
    $$PWD/mvcpublicdef.h \
    $$PWD/viewstyleproperties.h \
    $$PWD/sorter.h

SOURCES += \
    $$PWD/viewstyleproperties.cpp \
    $$PWD/sorter.cpp

FORMS +=

