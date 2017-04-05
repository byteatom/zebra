include(plugin.pri)

QT += gui

SOURCES += \
	$$PWD/implib/RegionCfger.cpp \
	$$PWD/implib/GeometryCfger.cpp

HEADERS += \
	$$PWD/implib/Cfger.h \
	$$PWD/implib/RegionCfger.h \
	$$PWD/implib/GeometryCfger.h

FORMS += \
	$$PWD/implib/RegionCfger.ui \
	$$PWD/implib/GeometryCfger.ui

