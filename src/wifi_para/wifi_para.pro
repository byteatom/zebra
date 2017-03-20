TARGET = wifi_para
TEMPLATE = app

include(../plugin.pri)

win32 {
RC_ICONS = wifi_para_512.ico
}

SOURCES +=\
	../lib/UtilWin.cpp \
	SsidValidator.cpp \
	PasswordValidator.cpp \
    WifiParaDlg.cpp \
    WifiPara.cpp

HEADERS  += \
	SsidValidator.h \
	PasswordValidator.h \
    WifiParaDlg.h

FORMS    += \
    WifiParaDlg.ui
