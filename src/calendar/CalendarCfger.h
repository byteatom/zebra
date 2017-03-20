#ifndef CALENDARCFGER_H
#define CALENDARCFGER_H

#include "RegionCfger.h"
#include "CalendarAttr.h"
#include "CalendarAttrCfger.h"

class CalendarCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit CalendarCfger(CalendarAttr *attr);
	virtual ~CalendarCfger();

private:
	CalendarAttr* attr = nullptr;
	CalendarAttrCfger* attrCfger = nullptr;
};

#endif // CALENDARCFGER_H
