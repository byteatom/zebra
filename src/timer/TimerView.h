#ifndef TIMERVIEW_H
#define TIMERVIEW_H

#include <QPixmap>

#include "RegionView.h"
#include "TimerAttr.h"

class TimerView : public RegionView
{
	Q_OBJECT
public:
	explicit TimerView(TimerAttr* attr);
	virtual ~TimerView() = default;

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	TimerAttr* attr = nullptr;
};

#endif // TIMERVIEW_H
