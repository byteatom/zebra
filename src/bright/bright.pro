TARGET = bright
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	../implib/Message.cpp \
	BrightDlg.cpp \
	BrightManager.cpp

HEADERS += \
	BrightDlg.h \
	BrightManager.h

FORMS += \
	BrightDlg.ui

RESOURCES +=
