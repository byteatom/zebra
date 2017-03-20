TARGET = serial
TEMPLATE = lib


include(../plugin.pri)

QT += serialport

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


SOURCES += \
	../implib/Message.cpp \
	Serial.cpp \
	SerialFactory.cpp \
	SerialCfger.cpp

HEADERS += \
	Serial.h \
	SerialFactory.h \
	SerialCfger.h

FORMS += \
	SerialCfger.ui
