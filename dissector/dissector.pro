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
    dissectors/frame.cpp \
    dissectors/protreemaker.cpp \
    dissectors/tcp_ip_protocol_family/ipv4.cpp \
    dissectors/tcp_ip_protocol_family/linklayer.cpp \
    dissectors/tcpipprotocolfamily.cpp \
    dissres/Converter.cpp \
    dissres/dissectresultbase.cpp \
    dissres/dissectresultcommonstream.cpp \
    dissres/dissectresutltframe.cpp \
    dissres/dissres.cpp \
    dissres/dissreseth.cpp \
    dissres/tcp_ip_protocol_family/dissectresultarp.cpp \
    dissres/tcp_ip_protocol_family/dissectresultipv4.cpp \
    dissres/tcp_ip_protocol_family/dissectresultipv6.cpp \
    dissres/tcp_ip_protocol_family/dissectresultlinklayer.cpp \
    dissres/tcp_ip_protocol_family/dissectresulttcp.cpp \
    dissres/tcp_ip_protocol_family/dissectresultudp.cpp \
    info/info.cpp \
    info/infoeth.cpp \
    loader.cpp \
    protree/protree.cpp \
    protree/protreenode.cpp \
    stream/stream.cpp \
    stream/streamindex.cpp \
    stream/streamitem.cpp \
    stream/streamrecorder.cpp \
    stream/streamtcp.cpp \
    units/bit.cpp \
    units/checksum.cpp \
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
    dissectors/frame.h \
    dissectors/protreemaker.h \
    dissectors/tcp_ip_protocol_family/ipv4.h \
    dissectors/tcp_ip_protocol_family/linklayer.h \
    dissectors/tcpipprotocolfamily.h \
    dissres/converter.h \
    dissres/dissectresultbase.h \
    dissres/dissectresultcommonstream.h \
    dissres/dissectresutltframe.h \
    dissres/dissres.h \
    dissres/dissreseth.h \
    dissres/tcp_ip_protocol_family/dissectresultarp.h \
    dissres/tcp_ip_protocol_family/dissectresultipv4.h \
    dissres/tcp_ip_protocol_family/dissectresultipv6.h \
    dissres/tcp_ip_protocol_family/dissectresultlinklayer.h \
    dissres/tcp_ip_protocol_family/dissectresulttcp.h \
    dissres/tcp_ip_protocol_family/dissectresultudp.h \
    info/info.h \
    info/infoeth.h \
    loader.h \
    protree/protree.h \
    protree/protreenode.h \
    \ \
    stream/stream.h \
    stream/streamindex.h \
    stream/streamitem.h \
    stream/streamrecorder.h \
    stream/streamtcp.h \
    units/bit.h \
    units/checksum.h \
    units/device.h \
    units/keys.h

LIBS += -lpcap

MOC_DIR = ./mocs
OBJECTS_DIR = ./objs
DESTDIR = ../lib

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    others/ethernet-addresses \
#    others/ethernet-well-know-addresses

RESOURCES += \
    resource/dissectorres.qrc
