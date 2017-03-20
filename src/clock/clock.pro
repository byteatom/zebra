TARGET = clock
TEMPLATE = lib


include(../region.pri)

INCLUDEPATH += $$PWD/../lib/sdl/inc
INCLUDEPATH += $$PWD/../implib/sdl

LIBS += -L$$PWD/../lib/sdl/lib -lSDL -lSDL_gfx -lSDL_ttf

SOURCES += Clock.cpp \
	ClockManager.cpp \
	ClockAttr.cpp \
	ClockView.cpp \
	ClockCfger.cpp \
	../implib/sdl/Common.c \
	../implib/sdl/LedBitmap.c \
	../implib/sdl/MapIntInt.c \
	KGWatch.c \
	ClockAttrCfger.cpp

HEADERS += Clock.h \
	ClockManager.h \
	ClockAttr.h \
	ClockView.h \
	ClockCfger.h \
	ClockAttrCfger.h

FORMS += \
	ClockAttrCfger.ui
