#ifndef OFFICEITEM_H
#define OFFICEITEM_H

#include <vector>

#include <QPixmap>
#include <QDateTime>

#include "AttrItem.h"
#include "FileItem.h"
#include "CacheItem.h"

class OfficeItem : public CacheItem
{
public:
	OfficeItem(ImageAttr* attr, const Type type, const QString& file, Json* jnode);
	virtual ~OfficeItem() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual bool editImpl() override;
	virtual void printImpl() override;
	void scan();

protected:
	virtual void scanImpl() = 0;

	FileItem fileItem;
	QDateTime cacheTime;
};

#endif // OFFICEITEM_H
