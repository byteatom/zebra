QT -= core gui

VERSION = 6.0.0.0

CONFIG += c++14

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
equals(TEMPLATE, "lib") {
	DESTDIR = $$OUT_PWD/bin
	DLLDESTDIR = $$PWD/../$$BUILD_TYPE/bin/plugin
}
equals(TEMPLATE, "app") {
	DESTDIR = $$PWD/../$$BUILD_TYPE/bin
}

win32-msvc {
#QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
#QMAKE_CXXFLAGS_STL_ON = /EHa
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
QMAKE_TARGET_PRODUCT = $${TARGET}
CONFIG(release, debug|release) {
	QMAKE_CXXFLAGS_RELEASE += /Zi /Oy-
	PDB_DIR = $$PWD/../pdb/$$VERSION
	mkpath($${PDB_DIR})
	QMAKE_LFLAGS_RELEASE += /DEBUG /PDB:"$${PDB_DIR}/$${TARGET}.pdb"
}
}

INCLUDEPATH += $$PWD/lib
INCLUDEPATH += $$PWD/lib/boost
INCLUDEPATH += $$PWD/lib/qlog/inc

LIBS += -L$$PWD/lib/boost/stage/x86/lib
LIBS += -L$$PWD/lib/qlog/lib/$$BUILD_TYPE

SOURCES +=
