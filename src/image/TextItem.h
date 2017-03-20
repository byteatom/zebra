#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QCoreApplication>

#include "AttrItem.h"

class TextItem : public AttrItem
{
	Q_DECLARE_TR_FUNCTIONS(TextItem)
public:
	TextItem(ImageAttr* attr, Json* jnode);
	virtual ~TextItem() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual bool editImpl() override;
	virtual void printImpl() override;

	QStr html;
	qreal letterSpace = 0;
	qreal lineSpace = 0;
	qreal paragraphSpace = 0;
};

#endif // TEXTITEM_H
