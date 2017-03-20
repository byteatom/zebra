TARGET = flash_text
TEMPLATE = lib


include(../region.pri)

INCLUDEPATH += $$PWD/../lib/sdl/inc
INCLUDEPATH += $$PWD/../implib/sdl

LIBS += -L$$PWD/../lib/sdl/lib -lSDL -lSDL_gfx -lSDL_ttf

SOURCES += FlashText.cpp \
		FlashTextManager.cpp \
		FlashTextAttr.cpp \
		FlashTextView.cpp \
		FlashTextCfger.cpp \
	../implib/sdl/Common.c \
	../implib/sdl/LedBitmap.c \
	../implib/sdl/MapIntInt.c \
	other/DominoEffect.c \
	other/DynamicTextEffectBase.c \
	other/ExcursionEffect.c \
	other/ExcursionReverseEffect.c \
	other/FlashBulbEffect.c \
	other/HeartBeatEffect.c \
	other/NeonLightEffect.c \
	other/RadioWaveEffect.c \
	other/RainbowDownEffect.c \
	other/RainbowEffect.c \
	other/RainbowLeftEffect.c \
	other/RainbowRightEffect.c \
	other/RainbowUpEffect.c \
	other/RandomColorEffect.c \
	other/RollOverEffect.c \
	other/SpringEffect.c \
	other/SwirlingEffect.c \
	other/WaltzEffect.c \
	../implib/sdl/DynamicBase.c \
	FlashColor.cpp \
	ColorWidget.cpp \
	FlashTextAttrCfger.cpp

HEADERS += FlashText.h \
		FlashTextManager.h \
		FlashTextAttr.h \
		FlashTextView.h \
		FlashTextCfger.h \
	other/DynamicTextEffectBase.h \
	FlashColor.h \
	ColorWidget.h \
	KGShineFont.h \
	FlashTextAttrCfger.h

FORMS += \
	ColorWidget.ui \
	FlashTextAttrCfger.ui

