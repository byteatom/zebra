TARGET = border
TEMPLATE = lib


include(../plugin.pri)

SOURCES += Border.cpp \
		BorderManager.cpp \
		BorderAttr.cpp \
		BorderCfger.cpp

HEADERS += Border.h \
		BorderManager.h \
		BorderAttr.h \
		BorderCfger.h

FORMS += \
		BorderCfger.ui
