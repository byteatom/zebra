TARGET = screen_attr
TEMPLATE = lib


include(../plugin.pri)

SOURCES += \
	../implib/Message.cpp \
    ScreenAttrDlg.cpp \
    ScreenAttrManager.cpp \
    ModuleEditor.cpp \
    ModuleWizard.cpp

HEADERS += \
    ScreenAttrDlg.h \
    ScreenAttrManager.h \
    ModuleEditor.h \
    ModuleWizard.h

FORMS += \
    ScreenAttrDlg.ui \
    ModuleEditor.ui \
    ModuleWizard.ui
