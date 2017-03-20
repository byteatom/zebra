#include "WordItem.h"

#include <QAxObject>
#include <QMessageBox>

#include "Util.h"
#include "UtilWin.h"

const QString WordItem::filter()
{
	return tr("Microsoft Word (*.doc *.docx)");
}

WordItem::WordItem(ImageAttr* attr, const QString& file, Json* jnode):
	OfficeItem{attr, Type::WORD, file, jnode}
{
}

void WordItem::scanImpl()
{
	QAxObject* word = new QAxObject{"Word.Application"};
	if (word->isNull()) {
		QMessageBox::critical(listWidget(), tr("Error"),
			tr("Failed to load Microsoft Word component.\n"
			   "Please check if it's installed correctly."));
		return;
	}
	Util::Guard wordGurad{[word](){
			word->dynamicCall("Quit()");
			delete word;
		}};
	word->setProperty("Visible", false);

	QAxObject *documents = word->querySubObject("Documents");
	if (!documents)  return;

	QAxObject *document = documents->querySubObject("Open(QString)", fileItem.file);
	if (!document)  return;
	Util::Guard docGurad{[document](){
			QList<QVariant> paraClose;
			paraClose << 0 << 1 << false;
			document->dynamicCall("Close()");
		}};

	QAxObject *selection = word->querySubObject("Selection");
	if (!selection) return;
	selection->dynamicCall("WholeStory()");
	int totleStart = selection->property("Start").toInt();
	int totleEnd = selection->property("End").toInt();
	int pageCount = selection->dynamicCall("Information(4)").toInt();

	for (int index = 1; index <= pageCount; ++index) {
		int pageStart = 0;
		int pageEnd = 0;
		if (1 == pageCount) {
			pageStart = totleStart;
			pageEnd = totleEnd;
		} else {
			QList<QVariant> paraGoto;
			paraGoto << 1 << 1 << index;
			QAxObject *rangePre	= selection->querySubObject("GoTo(QVariant,QVariant,QVariant)",	paraGoto);
			QAxObject *rangeNext = selection->querySubObject("GoToNext(1)");
			if (rangePre && rangeNext) {
				pageStart = rangePre->property("Start").toInt();
				if (pageCount == index) {
					pageEnd = totleEnd;
				} else {
					pageEnd	= rangeNext->property("Start").toInt();
				}
			}
		}

		QList<QVariant> paraRange;
		paraRange << pageStart << pageEnd;
		QAxObject *pageRange = document->querySubObject("Range(QVariant,QVariant)", paraRange);
		if (!pageRange) continue;
		pageRange->dynamicCall("CopyAsPicture()");

		QImage image = UtilWin::getClipboardEmf();
		if (image.isNull()) continue;
		CacheItem::add(QPixmap::fromImage(image));
	}
}
