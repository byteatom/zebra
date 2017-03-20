TARGET = update
TEMPLATE = lib

include(../plugin.pri)

INCLUDEPATH += $$PWD/../lib/WinSparkle/include
LIBS += -L$$PWD/../lib/WinSparkle/Release -lWinSparkle

SOURCES += \
	Update.cpp

HEADERS += \
	Update.h

FORMS +=
