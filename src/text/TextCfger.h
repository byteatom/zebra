#ifndef TEXTCFGER_H
#define TEXTCFGER_H

#include "RegionCfger.h"
#include "TextAttr.h"
#include "TextAttrCfger.h"

class TextCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit TextCfger(TextAttr *attr);
	virtual ~TextCfger();

private:
	TextAttr* attr = nullptr;
	TextAttrCfger* attrCfger = nullptr;
};

#endif // TEXTCFGER_H
