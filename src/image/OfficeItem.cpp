#include "OfficeItem.h"

#include <QFileInfo>
#include <QFile>

#include "UtilQt.h"
#include "UtilWin.h"
#include "ImageAttr.h"

OfficeItem::OfficeItem(ImageAttr* attr, const Type type, const QString& file, Json* jnode):
	CacheItem{attr, type, QFileInfo(file).fileName(), jnode},
	fileItem{file, jnode}
{
	for (auto& cache: caches) {
		QDateTime fileTime = QFileInfo(cache).lastModified();
		if (cacheTime < fileTime) cacheTime = fileTime;
	}
}

void OfficeItem::exportProjJson(Json& jnode)
{
	CacheItem::exportProjJson(jnode);
	fileItem.exportProjJson(jnode);
}

bool OfficeItem::editImpl()
{
	QDateTime time = QFileInfo(fileItem.file).lastModified();
	if (UtilWin::execWait(fileItem.file)) {
		return time !=  QFileInfo(fileItem.file).lastModified();
	} else
		return false;
}

void OfficeItem::printImpl()
{
	if (snaps.empty() ||
		QFileInfo(fileItem.file).lastModified() > cacheTime) {
		scan();
	}

	pages.clear();
	editingPage = 0;
	for (QPixmap& pixmap: snaps) {
		pages.push_back(UtilQt::scale(pixmap, UtilQt::FIT, attr->contentRect->size));
	}
}

void OfficeItem::scan()
{
	CacheItem::clear();
	cacheTime = QDateTime::currentDateTimeUtc();
	scanImpl();
}

