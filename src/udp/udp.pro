TARGET = udp
TEMPLATE = lib


include(../plugin.pri)

QT += network

gcc {
	#C++ Memory Validator
	#QMAKE_CFLAGS_DEBUG += -gstabs
	#QMAKE_CXXFLAGS_DEBUG += -gstabs
	#QMAKE_CFLAGS_DEBUG += -gcoff
	#QMAKE_CXXFLAGS_DEBUG += -gcoff

	#Dr.memory
	QMAKE_CFLAGS_DEBUG += -ggdb
	QMAKE_CXXFLAGS_DEBUG += -ggdb

	DESTDIR = $$PWD/../../ksdk/$$BUILD_TYPE
	DLLDESTDIR = $$PWD/../../ksdk/$$BUILD_TYPE
}


SOURCES += Udp.cpp \
	UdpFactory.cpp \
	../implib/Message.cpp \
	UdpCfger.cpp

HEADERS += Udp.h \
	UdpFactory.h \
	UdpCfger.h \
	../lib/QIpAddrEditor.h

FORMS += \
	UdpCfger.ui
