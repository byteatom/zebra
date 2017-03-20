#ifndef CLOCK_H
#define CLOCK_H

#include "ClockView.h"
#include "ClockCfger.h"
#include "ClockAttr.h"
#include "Region.h"

struct ClockData
{
	explicit ClockData(INodeBox* box, Json* jnode);
	~ClockData();

	ClockAttr* attr;
	ClockView* view;
	ClockCfger* cfger;
};

class Clock : private ClockData, public Region
{
public:
	explicit Clock(INodeBox* box, Json* jnode);

private:
	virtual ~Clock() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	using ClockData::attr;
	using ClockData::view;
	using ClockData::cfger;
};

#endif // CLOCK_H
