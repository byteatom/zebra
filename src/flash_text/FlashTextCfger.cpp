#include "FlashTextCfger.h"

FlashTextCfger::FlashTextCfger(FlashTextAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new FlashTextAttrCfger{this, attr}}
{
	append(attrCfger);
	appendBorder();
	appendGlow();
}

FlashTextCfger::~FlashTextCfger()
{
}
