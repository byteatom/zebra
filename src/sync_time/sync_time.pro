TARGET = sync_time
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	../implib/Message.cpp \
    SyncTimeDlg.cpp \
    SyncTimeManager.cpp

HEADERS += \
    SyncTimeDlg.h \
    SyncTimeManager.h

FORMS += \
    SyncTimeDlg.ui

RESOURCES +=
