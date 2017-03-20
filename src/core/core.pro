TARGET = core
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	base64/base64.c \
	json/arraylist.c \
	json/debug.c \
	json/json_object.c \
	json/json_tokener.c \
	json/json_util.c \
	json/linkhash.c \
	json/printbuf.c \
	kaler/const_char.c \
	kaler/core_export.c \
	kaler/file_ops.c \
	kaler/parse_acfcount.c \
	kaler/parse_acfshine.c \
	kaler/parse_acftemperature.c \
	kaler/parse_acftime.c \
	kaler/parse_acfwatch.c \
	kaler/parse_acfword.c \
	kaler/parse_display.c \
	kaler/parse_network.c \
	kaler/parse_program.c \
	kaler/parse_protocol.c \
	kaler/parse_scandot.c \
	kaler/parse_screen.c \
	kaler/parse_top.c \
	Core.cpp \
		kaler/parse_acfcolorful.c

HEADERS +=\
	base64/base64.h \
	json/arraylist.h \
	json/bits.h \
	json/config.h \
	json/debug.h \
	json/json.h \
	json/json_inttypes.h \
	json/json_object.h \
	json/json_object_private.h \
	json/json_tokener.h \
	json/json_util.h \
	json/linkhash.h \
	json/printbuf.h \
	kaler/core_export.h \
	kaler/file_ops.h \
	kaler/kaler_config.h \
	kaler/kaler_macro.h \
	kaler/kaler_struct.h \
	kaler/parse_acfcount.h \
	kaler/parse_acfshine.h \
	kaler/parse_acftemperature.h \
	kaler/parse_acftime.h \
	kaler/parse_acfwatch.h \
	kaler/parse_acfword.h \
	kaler/parse_display.h \
	kaler/parse_network.h \
	kaler/parse_program.h \
	kaler/parse_protocol.h \
	kaler/parse_scandot.h \
	kaler/parse_screen.h \
	kaler/parse_top.h \
	Core.h \
		kaler/parse_acfcolorful.h

