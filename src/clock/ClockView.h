#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include <QPixmap>

#include "RegionView.h"
#include "ClockAttr.h"

class ClockView : public RegionView
{
public:
	explicit ClockView(ClockAttr* attr);
	virtual ~ClockView() = default;

	QPixmap drawWatch();

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	ClockAttr* attr = nullptr;
	WatchAreaInput inputBK;
};

#endif // CLOCKVIEW_H
