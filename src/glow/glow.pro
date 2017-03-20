TARGET = glow
TEMPLATE = lib


include(../region.pri)

SOURCES += \
    Glow.cpp \
    GlowCfger.cpp \
    GlowManager.cpp \
    ../lib/UtilQt.cpp \
    GlowEffect.cpp \
    GlowEditor.cpp

HEADERS += \
    Glow.h \
    GlowCfger.h \
    GlowManager.h \
    GlowEffect.h \
    GlowEditor.h

FORMS += \
    GlowCfger.ui \
    GlowEditor.ui

