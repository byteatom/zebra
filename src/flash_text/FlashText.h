#ifndef FLASHTEXT_H
#define FLASHTEXT_H

#include "FlashTextView.h"
#include "FlashTextCfger.h"
#include "FlashTextAttr.h"
#include "Region.h"

struct FlashTextData
{
	explicit FlashTextData(INodeBox* box, Json* jnode);
	~FlashTextData();

	FlashTextAttr* attr;
	FlashTextView* view;
	FlashTextCfger* cfger;
};

class FlashText : private FlashTextData, public Region
{
public:
	explicit FlashText(INodeBox* box, Json* jnode);

private:
	virtual ~FlashText() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	using FlashTextData::attr;
	using FlashTextData::view;
	using FlashTextData::cfger;
};

#endif // FLASHTEXT_H
