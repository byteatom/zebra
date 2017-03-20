TARGET = kshow
TEMPLATE = app

include(..\plugin.pri)

INCLUDEPATH += $$PWD/../lib/CrashRpt

LIBS += -L$$PWD/../lib/CrashRpt -lCrashRpt1403

win32 {
	#RC_ICONS = $$PWD/../$$BUILD_TYPE/image/icon/$${TARGET}.ico
	#CONFIG -= embed_manifest_exe
	#RC_FILE = main.rc
}

msvc {
	QMAKE_LFLAGS += /MANIFEST:NO
	QMAKE_LFLAGS_EXE =
}

SOURCES += \
	../lib/UtilWin.cpp \
	kshow.cpp
