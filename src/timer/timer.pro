TARGET = timer
TEMPLATE = lib


include(../region.pri)

SOURCES += \
	Timer.cpp \
	TimerAttr.cpp \
	TimerCfger.cpp \
	TimerManager.cpp \
	TimerView.cpp \
	TimerAttrCfger.cpp \
    ../lib/UtilQt.cpp

HEADERS += \
	Timer.h \
	TimerView.h \
	TimerManager.h \
	TimerCfger.h \
	TimerAttr.h \
	TimerAttrCfger.h

FORMS += \
	TimerAttrCfger.ui
