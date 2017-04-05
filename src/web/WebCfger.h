#ifndef WEBCFGER_H
#define WEBCFGER_H

#include "RegionCfger.h"
#include "WebAttr.h"
#include "WebAttrCfger.h"

class WebCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit WebCfger(WebAttr *attr);
	virtual ~WebCfger();

private:
	WebAttr* attr = nullptr;
	WebAttrCfger* attrCfger = nullptr;
};

#endif // WEBCFGER_H
