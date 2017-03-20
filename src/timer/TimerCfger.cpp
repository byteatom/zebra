#include "TimerCfger.h"

TimerCfger::TimerCfger(TimerAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new TimerAttrCfger{this, attr}}
{
	append(attrCfger);
	appendBorder();
	appendGlow();
}

TimerCfger::~TimerCfger()
{
}



