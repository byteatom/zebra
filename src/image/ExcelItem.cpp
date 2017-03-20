#include "ExcelItem.h"

#include <QAxObject>
#include <QMessageBox>

#include "Util.h"
#include "UtilWin.h"

const QString ExcelItem::filter()
{
	return tr("Microsoft Excel (*.xls *.xlsx)");
}

ExcelItem::ExcelItem(ImageAttr* attr, const QString& file, Json* jnode):
	OfficeItem{attr, Type::EXCEL, file, jnode}
{
}

void ExcelItem::scanImpl()
{
	QAxObject* excel = new QAxObject{"Excel.Application"};
	if (excel->isNull()) {
		QMessageBox::critical(listWidget(), tr("Error"),
			tr("Failed to load Microsoft Excel component.\n"
			   "Please check if it's installed correctly."));
		return;
	}
	Util::Guard excelGurad{[excel](){
			excel->dynamicCall("Quit()");
			delete excel;
		}};
	excel->setProperty("Visible", false);

	QAxObject *books = excel->querySubObject("Workbooks");
	if (!books) return;
	Util::Guard booksGurad{[books](){
			books->dynamicCall("Close()");
		}};

	QAxObject *book = books->querySubObject("Open(QString)", fileItem.file);
	if (!book)  return;
	Util::Guard bookGurad{[book](){
			book->dynamicCall("Close()");
		}};

	QAxObject *sheets = book->querySubObject(/*"Sheets"*/"Worksheets");
	if (!sheets) return;

	int sheetCount = sheets->property("Count").toInt();
	if (sheetCount <= 0) return;

	for (int index = 1; index <= sheetCount; ++index) {
		QAxObject *sheet = sheets->querySubObject("Item(int)", index);
		if (!sheet) continue;

		QAxObject *usedRange = sheet->querySubObject("UsedRange");
		if (!usedRange) continue;
		usedRange->dynamicCall("CopyPicture()");

		QImage image = UtilWin::getClipboardEmf();
		if (image.isNull()) continue;
		CacheItem::add(QPixmap::fromImage(image));
	}
}

