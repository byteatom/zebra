TARGET = ksdk_vc
TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += skip_target_version_ext

CONFIG -= debug_and_release
CONFIG(debug, debug|release) {
	BUILD_TYPE = debug
}
CONFIG(release, debug|release) {
	BUILD_TYPE = release
}

OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/qrc
UI_DIR = $$OUT_PWD/ui
DESTDIR = $$PWD/../../ksdk/$$BUILD_TYPE

INCLUDEPATH += $$PWD/../ksdk
INCLUDEPATH += $$PWD/../fade
LIBS += -L$$PWD/../../ksdk/$$BUILD_TYPE
QMAKE_LFLAGS += libksdk.a

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01

SOURCES += main.cpp
