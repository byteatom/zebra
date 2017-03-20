#include "TextCfger.h"

TextCfger::TextCfger(TextAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new TextAttrCfger{this, attr}}
{
	append(attrCfger);

	QWidget* fadeCfger = attr->fade->newCfger();
	append(fadeCfger);

	appendBorder();
	appendGlow();
}

TextCfger::~TextCfger()
{
}



