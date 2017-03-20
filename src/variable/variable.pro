TARGET = variable
TEMPLATE = lib


include(../region.pri)

SOURCES += \
	Variable.cpp \
	VariableAttr.cpp \
	VariableCfger.cpp \
	VariableManager.cpp \
	VariableView.cpp \
	VariableAttrCfger.cpp

HEADERS += \
	Variable.h \
	VariableAttr.h \
	VariableCfger.h \
	VariableManager.h \
	VariableView.h \
	VariableAttrCfger.h

FORMS += \
	VariableAttrCfger.ui
