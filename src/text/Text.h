#ifndef TEXT_H
#define TEXT_H

#include "TextView.h"
#include "TextCfger.h"
#include "TextAttr.h"
#include "Region.h"

struct TextData
{
	explicit TextData(INodeBox* box, Json* jnode);
	~TextData();

	TextAttr* attr;
	TextView* view;
	TextCfger* cfger;
};

class Text : private TextData, public Region
{
public:
	explicit Text(INodeBox* box, Json* jnode);

private:
	virtual ~Text() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	virtual boost::signals2::connection pageObserve(PageObserver observer) override;
	virtual void pageFirst() override;
	virtual void pagePrevious() override;
	virtual void pageNext() override;
	virtual void pageLast() override;

	using TextData::attr;
	using TextData::view;
	using TextData::cfger;
};

#endif // TEXT_H
