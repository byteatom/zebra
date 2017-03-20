TARGET = calendar
TEMPLATE = lib


include(../region.pri)

SOURCES += \
	Calendar.cpp \
	CalendarAttr.cpp \
	CalendarCfger.cpp \
	CalendarManager.cpp \
	CalendarView.cpp \
	CalendarAttrCfger.cpp \
    ../lib/UtilQt.cpp

HEADERS += \
	Calendar.h \
	CalendarAttr.h \
	CalendarCfger.h \
	CalendarManager.h \
	CalendarView.h \
	CalendarAttrCfger.h

FORMS += \
	CalendarAttrCfger.ui
