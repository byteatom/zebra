TARGET = onoff
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	../implib/Message.cpp \
    OnOffDlg.cpp \
    OnOffManager.cpp

HEADERS += \
    OnOffDlg.h \
    OnOffManager.h

FORMS += \
    OnOffDlg.ui

RESOURCES +=
