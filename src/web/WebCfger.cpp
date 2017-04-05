#include "WebCfger.h"

WebCfger::WebCfger(WebAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new WebAttrCfger{this, attr}}
{
	append(attrCfger);
}

WebCfger::~WebCfger()
{
}



