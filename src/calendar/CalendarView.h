#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QPixmap>

#include "RegionView.h"
#include "CalendarAttr.h"

class CalendarView : public RegionView
{
public:
	explicit CalendarView(CalendarAttr* attr);
	virtual ~CalendarView() = default;

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	CalendarAttr* attr = nullptr;
};

#endif // CALENDARVIEW_H
