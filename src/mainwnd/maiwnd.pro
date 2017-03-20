TARGET = mainwnd
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	MainWnd.cpp \
	NodeItem.cpp

HEADERS += \
	../interface/ICommand.h \
	MainWnd.h \
	NodeItem.h

FORMS += \
	MainWnd.ui
