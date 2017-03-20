#ifndef FLASHTEXTATTR_H
#define FLASHTEXTATTR_H

#include <boost/signals2.hpp>

#include <QCoreApplication>
#include <QPixmap>

#include "QStr.h"
#include "RegionAttr.h"
#include "KGMarco.h"
#include "KGShineFont.h"

class FlashTextAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(FlashTextAttr)
public:
	explicit FlashTextAttr(INodeBox* box, Json* jnode);
	virtual ~FlashTextAttr();

	int style;
	int speed;
	int fontSize;
	int fontColor;
	bool bold;
	bool italic;
	bool underline;
	QStr text;
	QStr html;
	QStr fontName;

	ShineFontAreaInput input;
	ShineFontAreaOutput output;

	QPixmap snapshot;
	bool playing = false;

	void toInputAttr(KGCreateMode mode, const char *dir = "/");
	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // FLASHTEXTATTR_H
