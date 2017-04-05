TARGET = web
TEMPLATE = lib

QT += webenginewidgets

include(../region.pri)

SOURCES += \
	../lib/UtilQt.cpp \
	Web.cpp \
	WebAttr.cpp \
	WebAttrCfger.cpp \
	WebCfger.cpp \
	WebManager.cpp \
	WebView.cpp \
    WebItemEditor.cpp \
    WebItem.cpp \
    WebItemStore.cpp \
    WebItemListWidget.cpp

HEADERS += \
	Web.h \
	WebAttr.h \
	WebAttrCfger.h \
	WebCfger.h \
	WebManager.h \
	WebView.h \
    WebItemEditor.h \
    WebItem.h \
    WebItemStore.h \
    WebItemListWidget.h

FORMS += \
	WebAttrCfger.ui \
    WebItemEditor.ui \
    WebItemStore.ui
