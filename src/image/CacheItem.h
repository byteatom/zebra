#ifndef CACHEITEM_H
#define CACHEITEM_H

#include <vector>

#include <QPixmap>
#include <QDateTime>

#include "Json.h"
#include "AttrItem.h"

class CacheItem : public AttrItem
{
public:
	explicit CacheItem(ImageAttr* attr, const Type type, const QString& name, Json* jnode);
	virtual ~CacheItem();

	virtual void clearCaches() override;

	void add(const QPixmap& snap);
	void add(const QStr& file);
	void clear();
	void exportProjJson(Json& jnode);

	std::vector<QPixmap> snaps;
	std::vector<QStr> caches;
};

#endif // CACHEITEM_H
