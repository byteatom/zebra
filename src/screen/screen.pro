TARGET = screen
TEMPLATE = lib


include(../plugin.pri)

SOURCES += Screen.cpp \
	ScreenManager.cpp \
	ScreenCfger.cpp \
	ScreenView.cpp \
	ScreenAttr.cpp

HEADERS += Screen.h\
	ScreenManager.h \
	ScreenCfger.h \
	ScreenView.h \
	ScreenAttr.h \
	../implib/Cfger.h

FORMS += \
	ScreenCfger.ui
