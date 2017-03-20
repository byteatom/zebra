#ifndef TEXTATTR_H
#define TEXTATTR_H

#include <boost/signals2.hpp>

#include <QCoreApplication>
#include <QPixmap>
#include <QListWidget>

#include "RegionAttr.h"
#include "IRegion.h"

class ImageAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(ImageAttr)
public:
	explicit ImageAttr(INodeBox* box, Json* jnode);
	virtual ~ImageAttr();

	void userDelete();

	QListWidget *items;
	int applyId();
	int itemNameId = 0;
	int playingRow = 0;

	boost::signals2::connection pageObserve(IRegion::PageObserver observer);
	void pageFirst();
	void pagePrevious();
	void pageNext();
	void pageLast();

	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // TEXTATTR_H
