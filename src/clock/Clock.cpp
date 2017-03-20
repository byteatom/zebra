#include "Clock.h"

ClockData::ClockData(INodeBox* box, Json* jnode)
{
	attr = new ClockAttr{box, jnode},
	view = new ClockView{attr},
	cfger = new ClockCfger{attr};
}

ClockData::~ClockData()
{
	delete cfger;
	delete view;
	delete attr;
}

Clock::Clock(INodeBox *box, Json* jnode):
	ClockData{box, jnode},
	Region{attr, view, cfger}
{
}

void Clock::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Clock::play()
{
	Region::play();
}

void Clock::draw()
{
	Region::draw();
}

void Clock::stop()
{
	Region::stop();
}
