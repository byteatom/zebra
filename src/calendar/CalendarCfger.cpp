#include "CalendarCfger.h"

CalendarCfger::CalendarCfger(CalendarAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new CalendarAttrCfger{this, attr}}
{
	append(attrCfger);
	appendBorder();
	appendGlow();
}

CalendarCfger::~CalendarCfger()
{
}



