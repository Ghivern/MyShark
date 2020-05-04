QT       += core gui

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
    ui/devicelist.cpp \
    ui/dialogs/saveorclosefiledialog.cpp \
    ui/dialogs/stopwithoutanypacket.cpp \
    ui/dissectoroptions.cpp \
    ui/statistics/capturefileproperties.cpp \
    ui/widgets/arp.cpp \
    ui/widgets/ether.cpp \
    ui/widgets/frame.cpp \
    units/caphandle.cpp \
    units/dumper.cpp

HEADERS += \
    capturer/capturer.h \
    dissector/dissector.h \
    mainwindow.h \
    ui/devicelist.h \
    ui/dialogs/saveorclosefiledialog.h \
    ui/dialogs/stopwithoutanypacket.h \
    ui/dissectoroptions.h \
    ui/statistics/capturefileproperties.h \
    ui/widgets/arp.h \
    ui/widgets/ether.h \
    ui/widgets/frame.h \
    units/caphandle.h \ \
    units/dumper.h



FORMS += \
    ui/devicelist.ui \
    mainwindow.ui \
    ui/dialogs/saveorclosefiledialog.ui \
    ui/dialogs/stopwithoutanypacket.ui \
    ui/dissectoroptions.ui \
    ui/statistics/capturefileproperties.ui \
    ui/widgets/arp.ui \
    ui/widgets/ether.ui \
    ui/widgets/frame.ui

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
