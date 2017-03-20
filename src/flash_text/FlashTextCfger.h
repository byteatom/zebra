#ifndef FLASHTEXTCFGER_H
#define FLASHTEXTCFGER_H

#include "RegionCfger.h"
#include "FlashTextAttr.h"
#include "FlashTextAttrCfger.h"

class FlashTextCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit FlashTextCfger(FlashTextAttr *attr);
	virtual ~FlashTextCfger();

private:
	FlashTextAttr* attr = nullptr;
	FlashTextAttrCfger* attrCfger = nullptr;
};

#endif // FLASHTEXTCFGER_H
