#ifndef TEXTATTR_H
#define TEXTATTR_H

#include <vector>

#include <boost/signals2.hpp>

#include <QCoreApplication>
#include <QPixmap>

#include "RegionAttr.h"
#include "IRegion.h"
#include "IFade.h"

class TextAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(TextAttr)
public:
	explicit TextAttr(INodeBox* box, Json* jnode);
	virtual ~TextAttr();

	void print();

	QStr html;
	QRgb bgColor = qRgba(0, 0, 0, 0);
	Qt::AlignmentFlag align = Qt::AlignLeft;
	qreal letterSpace = 0;
	int strechHor = 0;
	int strechVer = 0;
	int offset = 0;
	QPixmap snapshot;
	std::vector<QPixmap> pages;
	int editingPage = 0;
	IFade* fade = nullptr;

	boost::signals2::connection pageObserve(IRegion::PageObserver observer);
	void pageFirst();
	void pagePrevious();
	void pageNext();
	void pageLast();

	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // TEXTATTR_H
