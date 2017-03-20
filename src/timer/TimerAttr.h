#ifndef TIMERATTR_H
#define TIMERATTR_H

#include <boost/signals2.hpp>

#include <QDateTime>
#include <QColor>

#include "RegionAttr.h"

class TimerAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(TimerAttr)
public:
	explicit TimerAttr(INodeBox* box, Json* jnode);
	virtual ~TimerAttr();

	bool elapse = false;
	QDateTime target{QDateTime::currentDateTime().addDays(1)};
	bool day = true;
	bool hour = true;
	bool minute = true;
	bool second = true;
	bool unit = true;
	QRgb unitColor = qRgba(255, 0, 0, 255);
	QRgb digitColor = qRgba(255, 0, 0, 255);
	QStr family;
	int point = 10;
	bool bold = false;
	bool italic = false;
	bool underline = false;
	bool extraText = false;
	QStr text;
	bool extraLine = false;
	QRgb textColor = qRgba(255, 0, 0, 255);

	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // TIMERATTR_H
