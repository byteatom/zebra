TARGET = fade
TEMPLATE = lib


include(../plugin.pri)

SOURCES += Fade.cpp \
	FadeManager.cpp \
	FadeCfger.cpp

HEADERS += Fade.h \
	FadeManager.h \
	FadeCfger.h \
    FadeStyle.h

FORMS += \
	FadeCfger.ui
