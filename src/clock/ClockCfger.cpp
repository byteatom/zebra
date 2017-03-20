#include "ClockCfger.h"

ClockCfger::ClockCfger(ClockAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new ClockAttrCfger{this, attr}}
{
	append(attrCfger);
	appendBorder();
	appendGlow();
}

ClockCfger::~ClockCfger()
{
}
