TARGET = program
TEMPLATE = lib


include(../plugin.pri)

SOURCES += Program.cpp \
	ProgramManager.cpp \
	ProgramView.cpp \
	ProgramCfger.cpp \
	ProgramAttr.cpp

HEADERS += Program.h \
	ProgramManager.h \
	ProgramView.h \
	ProgramCfger.h \
	ProgramAttr.h \
	../implib/Cfger.h

FORMS += \
	ProgramCfger.ui
