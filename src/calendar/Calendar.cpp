#include "Calendar.h"

CalendarData::CalendarData(INodeBox* box, Json* jnode)
{
	attr = new CalendarAttr{box, jnode},
	view = new CalendarView{attr},
	cfger = new CalendarCfger{attr};
}

CalendarData::~CalendarData()
{
	delete cfger;
	delete view;
	delete attr;
}

Calendar::Calendar(INodeBox *box, Json* jnode):
	CalendarData{box, jnode},
	Region{attr, view, cfger}
{
}

void Calendar::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Calendar::play()
{
	Region::play();
}

void Calendar::draw()
{
	Region::draw();
}

void Calendar::stop()
{
	Region::stop();
}
