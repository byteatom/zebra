TARGET = about
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	About.cpp \
	AboutDlg.cpp

HEADERS += \
	About.h \
	AboutDlg.h

FORMS += \
	AboutDlg.ui
