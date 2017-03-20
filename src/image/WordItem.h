#ifndef WORDITEM_H
#define WORDITEM_H

#include <QCoreApplication>

#include "OfficeItem.h"

class WordItem : public OfficeItem
{
	Q_DECLARE_TR_FUNCTIONS(WordItem)
public:
	static const QString filter();

	WordItem(ImageAttr* attr, const QString& file, Json* jnode);
	virtual ~WordItem() = default;

	virtual void scanImpl() override;
};

#endif // WORDITEM_H
