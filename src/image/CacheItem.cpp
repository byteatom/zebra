#include "CacheItem.h"

#include <QFileInfo>

#include "ISetting.h"
#include "UtilQt.h"

extern ISetting* setting;

CacheItem::CacheItem(ImageAttr* attr, const Type type, const QString& name, Json* jnode)
	: AttrItem{attr, type, name, jnode}
{
	if (jnode) {
		jtov(caches);
	}
	for (QStr& file: caches) {
		QPixmap pixmap{file};
		if (!pixmap.isNull()) {
			snaps.push_back(pixmap);
		} else {
			clear();
			break;
		}
	}
}

CacheItem::~CacheItem()
{
}

void CacheItem::clearCaches()
{
	for (QStr& file: caches) {
		QFile::remove(file);
	}
	caches.clear();
}

void CacheItem::add(const QPixmap& snap)
{
	snaps.push_back(snap);
	/*QStr file = ImageManager::setting->getUuidFile(ImageManager::setting->cacheDir(), ".png");
	caches.push_back(file);*/
}

void CacheItem::add(const QStr& file)
{
	caches.push_back(file);
	QPixmap snap{file};
	if (!snap.isNull()) snaps.push_back(snap);
}

void CacheItem::clear()
{
	snaps.clear();
	clearCaches();
}

void CacheItem::exportProjJson(Json& jnode)
{
	AttrItem::exportProjJson(jnode);
	clearCaches();
	for (auto& snap: snaps) {
		QStr file = UtilQt::uuidFile("data/cache/", ".png");
		snap.save(file);
		caches.push_back(file);
	}
	vtoj(caches);
}

