#ifndef CLOCKCFGER_H
#define CLOCKCFGER_H

#include "RegionCfger.h"

#include "ClockAttr.h"
#include "ClockAttrCfger.h"

class ClockCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit ClockCfger(ClockAttr *attr);
	virtual ~ClockCfger();

private:
	ClockAttr* attr = nullptr;
	ClockAttrCfger* attrCfger = nullptr;
};

#endif // CLOCKCFGER_H
