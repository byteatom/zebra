#ifndef SHEETITEM_H
#define SHEETITEM_H

#include <list>

#include <QCoreApplication>

#include "AttrItem.h"
#include "CacheItem.h"

class SheetItem : public CacheItem
{
	Q_DECLARE_TR_FUNCTIONS(SheetItem)
public:
	SheetItem(ImageAttr* attr, Json* jnode);
	virtual ~SheetItem() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual bool editImpl() override;
	virtual void printImpl() override;

	std::list<std::list<QStr>> html;
	std::list<int> rowHeight;
	std::list<int> colWidth;
};

#endif // SHEETITEM_H
