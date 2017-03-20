#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T02:25:01
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ksdk_test
TEMPLATE = app

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
DESTDIR = $$PWD/../../ksdk/$$BUILD_TYPE

INCLUDEPATH += $$PWD/../ksdk
INCLUDEPATH += $$PWD/../fade
LIBS += -L$$PWD/../../ksdk/$$BUILD_TYPE -llibksdk

gcc {
	#C++ Memory Validator
	#QMAKE_CFLAGS_DEBUG += -gstabs
	#QMAKE_CXXFLAGS_DEBUG += -gstabs
	#QMAKE_CFLAGS_DEBUG += -gcoff
	#QMAKE_CXXFLAGS_DEBUG += -gcoff

	#Dr.memory
	QMAKE_CFLAGS_DEBUG += -ggdb
	QMAKE_CXXFLAGS_DEBUG += -ggdb
}

SOURCES += main.cpp\
		MainDlg.cpp

HEADERS  += MainDlg.h

FORMS    += MainDlg.ui
