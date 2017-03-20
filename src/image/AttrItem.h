#ifndef ATTRITEM_H
#define ATTRITEM_H

#include <vector>

#include <boost/signals2.hpp>

#include <QListWidgetItem>
#include <QPixmap>

#include "QStr.h"
#include "IProjJson.h"
#include "ICoreJson.h"
#include "IFade.h"
#include "ImageAttr.h"

class AttrItem : public QListWidgetItem, public IProjJson, public ICoreJson
{
public:
	enum Type {TEXT, PICTURE, SHEET, WORD, PPT, EXCEL};

	AttrItem(ImageAttr* attr, const Type type, const QString& name, Json* jnode);
	virtual ~AttrItem();

	virtual void clearCaches();

	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jnode) override;

	bool edit();
	void print();

	QStr name;
	IFade* fade = nullptr;
	std::vector<QPixmap> pages;
	int editingPage = 0;
	int playingPage = 0;
protected:
	virtual bool editImpl() = 0;
	virtual void printImpl() = 0;

	ImageAttr* attr;
	Type type;
};

#endif // ATTRITEM_H
