#ifndef PPTITEM_H
#define PPTITEM_H


#include <QCoreApplication>

#include "OfficeItem.h"

class PptItem : public OfficeItem
{
	Q_DECLARE_TR_FUNCTIONS(PptItem)
public:
	static const QString filter();

	PptItem(ImageAttr* attr, const QString& file, Json* jnode);
	virtual ~PptItem() = default;

	virtual void scanImpl() override;
};

#endif // PPTITEM_H
