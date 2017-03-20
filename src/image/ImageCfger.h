#ifndef TEXTCFGER_H
#define TEXTCFGER_H

#include "RegionCfger.h"
#include "ImageAttr.h"
#include "ImageAttrCfger.h"

class ImageCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit ImageCfger(ImageAttr *attr);
	virtual ~ImageCfger();

private:
	ImageAttr *attr = nullptr;
	ImageAttrCfger* attrCfger = nullptr;
};

#endif // TEXTCFGER_H
