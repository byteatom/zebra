#ifndef PICTUREITEM_H
#define PICTUREITEM_H

#include <QCoreApplication>

#include "AttrItem.h"
#include "FileItem.h"

class PictureItem : public AttrItem
{
	Q_DECLARE_TR_FUNCTIONS(PictureItem)
public:
	static const QString filter();

	PictureItem(ImageAttr* attr, const QString& file, Json* jnode);
	virtual ~PictureItem() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual bool editImpl() override;
	virtual void printImpl() override;

	FileItem fileItem;
};

#endif // PICTUREITEM_H
