#ifndef CALENDAR_H
#define CALENDAR_H

#include "CalendarView.h"
#include "CalendarCfger.h"
#include "CalendarAttr.h"
#include "Region.h"

struct CalendarData
{
	explicit CalendarData(INodeBox* box, Json* jnode);
	~CalendarData();

	CalendarAttr* attr;
	CalendarView* view;
	CalendarCfger* cfger;
};

class Calendar : private CalendarData, public Region
{
public:
	explicit Calendar(INodeBox* box, Json* jnode);

private:
	virtual ~Calendar() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	using CalendarData::attr;
	using CalendarData::view;
	using CalendarData::cfger;
};

#endif // CALENDAR_H
