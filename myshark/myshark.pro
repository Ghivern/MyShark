QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    capturer/capturer.cpp \
    dissector/dissector.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/dialogs/saveorclosefiledialog.cpp \
    ui/dialogs/stopwithoutanypacket.cpp \
    ui/dissectoroptions.cpp \
    ui/maindisplaywidget/displayinterfaces.cpp \
    ui/maindisplaywidget/displayrawdata.cpp \
    ui/maindisplaywidget/displaytable.cpp \
    ui/maindisplaywidget/displaytree.cpp \
    ui/statistics/ProtocolHierarchy/hierarchynode.cpp \
    ui/statistics/ProtocolHierarchy/hierarchytree.cpp \
    ui/statistics/ProtocolHierarchy/protocolhierarchy.cpp \
    ui/statistics/capturefileproperties.cpp \
    ui/statistics/conversations/conversations.cpp \
    ui/statistics/conversations/conversationtablewidget.cpp \
    ui/statistics/endpoints/endpoints.cpp \
    ui/statistics/endpoints/endpointstablewidget.cpp \
    ui/statistics/iograph/iographchart.cpp \
    ui/statistics/packetlengths/packetlength.cpp \
    ui/statusbar/displayproportion.cpp \
    ui/widgets/arp.cpp \
    ui/widgets/ether.cpp \
    ui/widgets/frame.cpp \
    ui/widgets/ipv4.cpp \
    ui/widgets/tcp.cpp \
    ui/widgets/udp.cpp \
    units/caphandle.cpp \
    units/displayfilter.cpp \
    units/dumper.cpp

HEADERS += \
    capturer/capturer.h \
    dissector/dissector.h \
    mainwindow.h \
    ui/dialogs/saveorclosefiledialog.h \
    ui/dialogs/stopwithoutanypacket.h \
    ui/dissectoroptions.h \
    ui/maindisplaywidget/displayinterfaces.h \
    ui/maindisplaywidget/displayrawdata.h \
    ui/maindisplaywidget/displaytable.h \
    ui/maindisplaywidget/displaytree.h \
    ui/statistics/ProtocolHierarchy/hierarchynode.h \
    ui/statistics/ProtocolHierarchy/hierarchytree.h \
    ui/statistics/ProtocolHierarchy/protocolhierarchy.h \
    ui/statistics/capturefileproperties.h \
    ui/statistics/conversations/conversations.h \
    ui/statistics/conversations/conversationtablewidget.h \
    ui/statistics/endpoints/endpoints.h \
    ui/statistics/endpoints/endpointstablewidget.h \
    ui/statistics/iograph/iographchart.h \
    ui/statistics/packetlengths/packetlength.h \
    ui/statusbar/displayproportion.h \
    ui/widgets/arp.h \
    ui/widgets/ether.h \
    ui/widgets/frame.h \
    ui/widgets/ipv4.h \
    ui/widgets/tcp.h \
    ui/widgets/udp.h \
    units/caphandle.h \ \
    units/displayfilter.h \
    units/dumper.h



FORMS += \
    mainwindow.ui \
    ui/dialogs/saveorclosefiledialog.ui \
    ui/dialogs/stopwithoutanypacket.ui \
    ui/dissectoroptions.ui \
    ui/statistics/ProtocolHierarchy/protocolhierarchy.ui \
    ui/statistics/capturefileproperties.ui \
    ui/statistics/conversations/conversations.ui \
    ui/statistics/endpoints/endpoints.ui \
    ui/statistics/packetlengths/packetlength.ui \
    ui/statusbar/displayproportion.ui \
    ui/widgets/arp.ui \
    ui/widgets/ether.ui \
    ui/widgets/frame.ui \
    ui/widgets/ipv4.ui \
    ui/widgets/tcp.ui \
    ui/widgets/udp.ui

LIBS += -lpcap
LIBS += -L../lib -ldissector

INCLUDEPATH += ~/gitRepositories/MyShark/dissector/

QMAKE_RPATHDIR += ../lib

MOC_DIR = ./mocs
OBJECTS_DIR = ./objs
DESTDIR = ../bin


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/mysharkres.qrc
