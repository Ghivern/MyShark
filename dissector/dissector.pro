QT -= gui

TEMPLATE = lib
DEFINES += DISSECTOR_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dissectors/dissectoreth/dissectorarp.cpp \
    dissectors/dissectorbase.cpp \
    dissectors/dissectoreth/dissectoreth.cpp \
    dissectors/dissectoreth/dissectoripv6.cpp \
    dissectors/dissectorframe.cpp \
    dissectors/dissectoreth/dissectorip.cpp \
    dissectors/dissectoreth/dissectortcp.cpp \
    dissectors/dissectoreth/dissectorudp.cpp \
    dissres/dissres.cpp \
    dissres/dissreseth.cpp \
    info/info.cpp \
    info/infoeth.cpp \
    loader.cpp \
    protree/protree.cpp \
    protree/protreenode.cpp \
    stream/tustream.cpp \
    units/device.cpp

HEADERS += \
    dissector_global.h \
    dissectors/dissectoreth/dissectorarp.h \
    dissectors/dissectorbase.h \
    dissectors/dissectoreth/dissectoreth.h \
    dissectors/dissectoreth/dissectoripv6.h \
    dissectors/dissectorframe.h \
    dissectors/dissectoreth/dissectorip.h \
    dissectors/dissectoreth/dissectortcp.h \
    dissectors/dissectoreth/dissectorudp.h \
    dissectors/dissectoreth/eth_header.h \
    dissres/dissres.h \
    dissres/dissreseth.h \
    info/info.h \
    info/infoeth.h \
    loader.h \
    protree/protree.h \
    protree/protreenode.h \
    \ \
    stream/tustream.h \
    units/device.h

LIBS += -lpcap

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
