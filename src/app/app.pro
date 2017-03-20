TARGET = Zebra
TEMPLATE = app

include(..\base.pri)

DESTDIR = $$PWD/../../$$BUILD_TYPE

CONFIG -= app_bundle
CONFIG -= qt

#DEFINES+=PRODUCT_NAME=L\"\"\"$$TARGET\"\"\"

INCLUDEPATH += $$PWD/../interface

LIBS += -lShlwapi

SOURCES += \
	app.cpp

win32 {
#RC_ICONS = $$PWD/../$$BUILD_TYPE/image/icon/$${TARGET}.ico
}

HEADERS +=

