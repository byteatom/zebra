TARGET = comm
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	CommCfger.cpp \
	../implib/Message.cpp \
    Comm.cpp

HEADERS += \
	CommCfger.h \
    Comm.h

FORMS += \
	CommCfger.ui
