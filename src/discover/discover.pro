TARGET = discover
TEMPLATE = lib


include(../plugin.pri)

QT += serialport network

SOURCES += \
	../implib/Message.cpp \
	SerialPort.cpp \
	SocketUdp.cpp \
    DiscoverDlg.cpp \
    DiscoverManager.cpp \
    CardItem.cpp

HEADERS += \
	SerialPort.h \
	SocketUdp.h \
    DiscoverDlg.h \
    DiscoverManager.h \
    CardItem.h

FORMS += \
    DiscoverDlg.ui

RESOURCES +=
