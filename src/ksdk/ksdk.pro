#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T15:58:47
#
#-------------------------------------------------

QT += widgets serialport
VERSION = 6.0.0.0

TARGET = ksdk
TEMPLATE = lib

CONFIG += c++14

CONFIG += skip_target_version_ext

CONFIG -= debug_and_release
CONFIG(debug, debug|release) {
	BUILD_TYPE = debug
}
CONFIG(release, debug|release) {
	BUILD_TYPE = release
}

OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/qrc
UI_DIR = $$OUT_PWD/ui
DESTDIR = $$PWD/../../ksdk/$$BUILD_TYPE

DEFINES += KSDK_LIB

INCLUDEPATH += $$PWD/../lib
INCLUDEPATH += $$PWD/../lib/boost
INCLUDEPATH += $$PWD/../implib
INCLUDEPATH += $$PWD/../interface
INCLUDEPATH += $$PWD/../core
INCLUDEPATH += $$PWD/../fade

gcc {
	#C++ Memory Validator
	#QMAKE_CFLAGS_DEBUG += -gstabs
	#QMAKE_CXXFLAGS_DEBUG += -gstabs
	#QMAKE_CFLAGS_DEBUG += -gcoff
	#QMAKE_CXXFLAGS_DEBUG += -gcoff

	#Dr.memory
	QMAKE_CFLAGS_DEBUG += -ggdb
	QMAKE_CXXFLAGS_DEBUG += -ggdb
}

#QMAKE_LFLAGS += -Wl,--output-def,$${TARGET}.def
#lib.exe /machine:i386 /def:ksdk.def

SOURCES += Ksdk.cpp \
	../implib/Message.cpp \
	../core/base64/base64.c \
	../core/json/arraylist.c \
	../core/json/debug.c \
	../core/json/json_object.c \
	../core/json/json_tokener.c \
	../core/json/json_util.c \
	../core/json/linkhash.c \
	../core/json/printbuf.c \
	../core/kaler/const_char.c \
	../core/kaler/core_export.c \
	../core/kaler/file_ops.c \
	../core/kaler/parse_acfcount.c \
	../core/kaler/parse_acfshine.c \
	../core/kaler/parse_acftemperature.c \
	../core/kaler/parse_acftime.c \
	../core/kaler/parse_acfwatch.c \
	../core/kaler/parse_acfword.c \
	../core/kaler/parse_display.c \
	../core/kaler/parse_network.c \
	../core/kaler/parse_program.c \
	../core/kaler/parse_protocol.c \
	../core/kaler/parse_scandot.c \
	../core/kaler/parse_screen.c \
	../core/kaler/parse_top.c \
	../core/kaler/parse_acfcolorful.c \
	KsdkImpl.cpp \
	KsdkWorker.cpp

HEADERS += Ksdk.h \
	../core/base64/base64.h \
	../core/json/arraylist.h \
	../core/json/bits.h \
	../core/json/config.h \
	../core/json/debug.h \
	../core/json/json.h \
	../core/json/json_inttypes.h \
	../core/json/json_object.h \
	../core/json/json_object_private.h \
	../core/json/json_tokener.h \
	../core/json/json_util.h \
	../core/json/linkhash.h \
	../core/json/printbuf.h \
	../core/kaler/core_export.h \
	../core/kaler/file_ops.h \
	../core/kaler/kaler_config.h \
	../core/kaler/kaler_macro.h \
	../core/kaler/kaler_struct.h \
	../core/kaler/parse_acfcolorful.h \
	../core/kaler/parse_acfcount.h \
	../core/kaler/parse_acfshine.h \
	../core/kaler/parse_acftemperature.h \
	../core/kaler/parse_acftime.h \
	../core/kaler/parse_acfwatch.h \
	../core/kaler/parse_acfword.h \
	../core/kaler/parse_display.h \
	../core/kaler/parse_network.h \
	../core/kaler/parse_program.h \
	../core/kaler/parse_protocol.h \
	../core/kaler/parse_scandot.h \
	../core/kaler/parse_screen.h \
	../core/kaler/parse_top.h \
	KsdkImpl.h \
	KsdkWorker.h
