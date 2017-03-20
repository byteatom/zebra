TARGET = cluster
TEMPLATE = lib


include(../plugin.pri)
include(../lib/xlsx/qtxlsx.pri)

QT += network

SOURCES += \
	Cluster.cpp \
	ClusterDlg.cpp \
	EditorDlg.cpp \
	GroupDelegate.cpp \
	ScreenDelegate.cpp \
	../implib/Message.cpp \
	StatusItem.cpp \
	SelGroupsDlg.cpp \
	IdDelegate.cpp \
    ImportInfo.cpp \
    ../lib/UtilQt.cpp

HEADERS += \
	Cluster.h \
	ClusterDlg.h \
	EditorDlg.h \
	GroupDelegate.h \
	ScreenDelegate.h \
	StatusItem.h \
	SelGroupsDlg.h \
	IdDelegate.h \
    ImportInfo.h

FORMS += \
	ClusterDlg.ui \
	EditorDlg.ui \
	SelGroupsDlg.ui \
    ImportInfo.ui
