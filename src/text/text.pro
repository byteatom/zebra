TARGET = text
TEMPLATE = lib


include(../region.pri)

SOURCES += Text.cpp \
	TextManager.cpp \
	TextAttr.cpp \
	TextView.cpp \
	TextCfger.cpp \
	TextAttrCfger.cpp \
    ../lib/UtilQt.cpp

HEADERS += Text.h \
	TextManager.h \
	TextAttr.h \
	TextView.h \
	TextCfger.h \
	TextAttrCfger.h

FORMS += \
	TextAttrCfger.ui
