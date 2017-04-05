#include "TextCfger.h"

TextCfger::TextCfger(TextAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new TextAttrCfger{this, attr}}
{
	append(attrCfger);

	if (attr->fade) append(attr->fade->newCfger());

	appendBorder();
	appendGlow();
}

TextCfger::~TextCfger()
{
}



