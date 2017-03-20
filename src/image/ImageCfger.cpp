#include "ImageCfger.h"

ImageCfger::ImageCfger(ImageAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new ImageAttrCfger{this, attr}}
{
	append(attrCfger);
	appendBorder();
	appendGlow();
}

ImageCfger::~ImageCfger()
{
}



