TARGET = feedback
TEMPLATE = lib


include(../plugin.pri)

QT += network

DEFINES += SMTP_BUILD LIBZIPPP_EXPORTS


INCLUDEPATH += ../lib/libzip ../lib/libzippp
LIBS += -L$$PWD/../lib/libzip -lzip

SOURCES += Feedback.cpp \
	FeedbackDlg.cpp \
	FeedbackEditor.cpp \
	Mailer.cpp \
	smtp/emailaddress.cpp \
	smtp/mimeattachment.cpp \
	smtp/mimecontentformatter.cpp \
	smtp/mimefile.cpp \
	smtp/mimehtml.cpp \
	smtp/mimeinlinefile.cpp \
	smtp/mimemessage.cpp \
	smtp/mimemultipart.cpp \
	smtp/mimepart.cpp \
	smtp/mimetext.cpp \
	smtp/quotedprintable.cpp \
	smtp/smtpclient.cpp \
	../lib/UtilWin.cpp \
	../lib/UtilQt.cpp \
	../lib/libzippp/libzippp.cpp

HEADERS += Feedback.h \
	FeedbackDlg.h \
	FeedbackEditor.h \
	Mailer.h \
	smtp/emailaddress.h \
	smtp/mimeattachment.h \
	smtp/mimecontentformatter.h \
	smtp/mimefile.h \
	smtp/mimehtml.h \
	smtp/mimeinlinefile.h \
	smtp/mimemessage.h \
	smtp/mimemultipart.h \
	smtp/mimepart.h \
	smtp/mimetext.h \
	smtp/quotedprintable.h \
	smtp/smtpclient.h \
	smtp/smtpexports.h \
	smtp/SmtpMime \
    FeedLog.h

FORMS += \
	FeedbackDlg.ui
