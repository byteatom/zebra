#ifndef TIMER_H
#define TIMER_H

#include "TimerView.h"
#include "TimerCfger.h"
#include "TimerAttr.h"
#include "Region.h"

struct TimerData
{
	explicit TimerData(INodeBox* box, Json* jnode);
	~TimerData();

	TimerAttr* attr;
	TimerView* view;
	TimerCfger* cfger;
};

class Timer : private TimerData, public Region
{
public:
	explicit Timer(INodeBox* box, Json* jnode);

private:
	virtual ~Timer() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	using TimerData::attr;
	using TimerData::view;
	using TimerData::cfger;
};

#endif // TIMER_H
