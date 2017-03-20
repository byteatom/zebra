TARGET = send
TEMPLATE = lib


include(../plugin.pri)

SOURCES += Send.cpp \
	SendDlg.cpp \
	../implib/Message.cpp \
	../lib/UtilWin.cpp \
    ../lib/UtilQt.cpp

HEADERS += Send.h \
	SendDlg.h

FORMS += \
	SendDlg.ui
