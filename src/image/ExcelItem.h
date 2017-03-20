#ifndef EXCELITEM_H
#define EXCELITEM_H


#include <QCoreApplication>

#include "OfficeItem.h"

class ExcelItem : public OfficeItem
{
	Q_DECLARE_TR_FUNCTIONS(ExcelItem)
public:
	static const QString filter();

	ExcelItem(ImageAttr* attr, const QString& file, Json* jnode);
	virtual ~ExcelItem() = default;

	virtual void scanImpl() override;
};

#endif // EXCELITEM_H
