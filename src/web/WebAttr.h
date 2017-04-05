#ifndef WEBATTR_H
#define WEBATTR_H

#include <QCoreApplication>

#include "RegionAttr.h"
#include "WebItem.h"
#include "IRegion.h"

class WebAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(WebAttr)
public:
	explicit WebAttr(INodeBox* box, Json* jnode);
	virtual ~WebAttr() = default;

	boost::signals2::connection pageObserve(IRegion::PageObserver observer);
	void pageFirst();
	void pagePrevious();
	void pageNext();
	void pageLast();

	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jnode) override{ return QString();}

	WebItems items;
	int editIndex;
};

#endif // WEBATTR_H
