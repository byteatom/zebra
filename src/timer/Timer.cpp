#include "Timer.h"

TimerData::TimerData(INodeBox* box, Json* jnode)
{
	attr = new TimerAttr{box, jnode},
	view = new TimerView{attr},
	cfger = new TimerCfger{attr};
}

TimerData::~TimerData()
{
	delete cfger;
	delete view;
	delete attr;
}

Timer::Timer(INodeBox *box, Json* jnode):
	TimerData{box, jnode},
	Region{attr, view, cfger}
{
}

void Timer::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Timer::play()
{
	Region::play();
}

void Timer::draw()
{
	Region::draw();
}

void Timer::stop()
{
	Region::stop();
}
