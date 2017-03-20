TARGET = image
TEMPLATE = lib


include(../region.pri)

QT += axcontainer

SOURCES += ImageManager.cpp \
	ImageAttr.cpp \
	ImageView.cpp \
	ImageCfger.cpp \
	FileItem.cpp \
	PictureItem.cpp \
	TextItem.cpp \
	AttrItem.cpp \
	Image.cpp \
	TextEditor.cpp \
	PictureEditor.cpp \
	WordItem.cpp \
	OfficeItem.cpp \
	ExcelItem.cpp \
	PptItem.cpp \
	SheetEditor.cpp \
	SheetItem.cpp \
	SheetCell.cpp \
	SheetCellEditor.cpp \
	CacheItem.cpp \
	ImageAttrCfger.cpp \
	../lib/UtilQt.cpp \
	../lib/UtilWin.cpp

HEADERS += ImageManager.h \
	ImageAttr.h \
	ImageView.h \
	ImageCfger.h \
	AttrItem.h \
	PictureItem.h \
	TextItem.h \
	FileItem.h \
	Image.h \
	TextEditor.h \
	PictureEditor.h \
	WordItem.h \
	OfficeItem.h \
	ExcelItem.h \
	PptItem.h \
	SheetEditor.h \
	SheetItem.h \
	SheetCell.h \
	SheetCellEditor.h \
	CacheItem.h \
	ImageAttrCfger.h

FORMS += \
	TextEditor.ui \
	PictureEditor.ui \
	SheetEditor.ui \
	ImageAttrCfger.ui
