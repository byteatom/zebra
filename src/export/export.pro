TARGET = export
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	Export.cpp \
	ExportDlg.cpp \
	../lib/UtilWin.cpp

HEADERS += \
	Export.h \
	ExportDlg.h

FORMS += \
	ExportDlg.ui
