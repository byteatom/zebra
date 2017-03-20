#ifndef TIMERCFGER_H
#define TIMERCFGER_H

#include "RegionCfger.h"
#include "TimerAttr.h"
#include "TimerAttrCfger.h"

class TimerCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit TimerCfger(TimerAttr *attr);
	virtual ~TimerCfger();

private:
	TimerAttr* attr = nullptr;
	TimerAttrCfger* attrCfger = nullptr;
};

#endif // TIMERCFGER_H
