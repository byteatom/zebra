TARGET = background
TEMPLATE = lib


include(../plugin.pri)

INCLUDEPATH += $$PWD/../lib/sdl/inc
INCLUDEPATH += $$PWD/../implib/sdl
INCLUDEPATH += $$PWD/other

LIBS += -L$$PWD/../lib/sdl/lib -lSDL -lSDL_gfx -lSDL_ttf

SOURCES += Background.cpp \
	BackgroundManager.cpp \
	BackgroundAttr.cpp \
	BackgroundCfger.cpp \
	../implib/sdl/Common.c \
	../implib/sdl/LedBitmap.c \
	../implib/sdl/MapIntInt.c \
	other/ArrowEffect.c \
	other/BabysbreathEffect.c \
	other/BlockEffect.c \
	other/CircleEffect.c \
	other/CirqueEffect.c \
	other/CrewelScanEffect.c \
	other/DiamondEffect.c \
	other/DynamicBackgroundEffectBase.c \
	other/FaceToFaceSlipLineEffect.c \
	other/FivePointedStarEffect.c \
	other/SideswaySlipLineEffect.c \
	other/SingleScanEffect.c \
	other/SnowEffect.c \
	other/TileEffectBase.c \
	other/DynamicBackgroundElement.c \
	../implib/sdl/DynamicBase.c \
	FreestyleDialog.cpp

HEADERS += Background.h \
	BackgroundManager.h \
	BackgroundAttr.h \
	BackgroundCfger.h \
	other/DynamicBackgroundEffectBase.h \
	other/TileEffectBase.h \
	FreestyleDialog.h \
	../implib/SelectLabel.h

FORMS += \
	BackgroundCfger.ui \
	FreestyleDialog.ui

unix {
	target.path = /usr/lib
	INSTALLS += target
}

DISTFILES += \
	SDL.lib \
	SDL_gfx.lib
