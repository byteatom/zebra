#include "PptItem.h"

#include <QAxObject>
#include <QMessageBox>
#include <QDir>

#include "Util.h"
#include "UtilQt.h"
#include "ISetting.h"

extern ISetting* setting;

const QString PptItem::filter()
{
	return tr("Microsoft PowerPoint (*.ppt *.pptx)");
}

PptItem::PptItem(ImageAttr* attr, const QString& file, Json* jnode):
	OfficeItem{attr, Type::PPT, file, jnode}
{
}

void PptItem::scanImpl()
{
	QAxObject* ppt = new QAxObject{"PowerPoint.Application"};
	if (ppt->isNull()) {
		QMessageBox::critical(listWidget(), tr("Error"),
			tr("Failed to load Microsoft PowerPoint component.\n"
			   "Please check if it's installed correctly."));
		return;
	}
	Util::Guard pptGurad{[ppt](){
			ppt->dynamicCall("Quit()");
			delete ppt;
		}};
	//ppt->setProperty("Visible", false);

	QAxObject *presentations = ppt->querySubObject("Presentations");
	if (!presentations) return;

	QAxObject *presentation = presentations->querySubObject("Open(QString,bool,bool,bool)",
														 fileItem.file, true, true, false);
	if (!presentation) return;
	Util::Guard presentationGurad{[presentation](){
			presentation->dynamicCall("Close()");
		}};

	QAxObject *slides = presentation->querySubObject("Slides");
	if (!slides) return;

	int slideCount = slides->property("Count").toInt();
	if (slideCount <= 0) return;

	for (int index = 1; index <= slideCount; ++index) {
		QAxObject *slide = slides->querySubObject("Item(int)", index);
		if (!slide) continue;

		QStr cache = UtilQt::uuidFile("data/cache/", ".png");

		slide->dynamicCall("Export(QString,QString)", QDir::toNativeSeparators(cache), "PNG");

		CacheItem::add(cache);
	}
}

