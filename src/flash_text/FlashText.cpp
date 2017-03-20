#include "FlashText.h"

FlashTextData::FlashTextData(INodeBox* box, Json* jnode)
{
	attr = new FlashTextAttr{box, jnode},
	view = new FlashTextView{attr},
	cfger = new FlashTextCfger{attr};
}

FlashTextData::~FlashTextData()
{
	delete cfger;
	delete view;
	delete attr;
}

FlashText::FlashText(INodeBox *box, Json* jnode):
	FlashTextData{box, jnode},
	Region{attr, view, cfger}
{
}

void FlashText::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void FlashText::play()
{
	Region::play();
}

void FlashText::draw()
{
	Region::draw();
}

void FlashText::stop()
{
	Region::stop();
}
