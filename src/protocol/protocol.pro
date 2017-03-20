TARGET = protocol
TEMPLATE = lib


include(../plugin.pri)

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

SOURCES += Protocol.cpp \
	ProtocolManager.cpp \
	../implib/Message.cpp

HEADERS += Protocol.h \
	ProtocolManager.h
