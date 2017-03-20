#ifndef TEXT_H
#define TEXT_H

#include "ImageView.h"
#include "ImageCfger.h"
#include "ImageAttr.h"
#include "Region.h"

struct ImageData
{
	explicit ImageData(INodeBox* box, Json* jnode);
	~ImageData();

	ImageAttr* attr;
	ImageView* view;
	ImageCfger* cfger;
};

class Image : private ImageData, public Region
{
public:
	explicit Image(INodeBox* box, Json* jnode);

private:
	virtual ~Image() = default;

	virtual void userDelete() override;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	virtual boost::signals2::connection pageObserve(PageObserver observer) override;
	virtual void pageFirst() override;
	virtual void pagePrevious() override;
	virtual void pageNext() override;
	virtual void pageLast() override;

	using ImageData::attr;
	using ImageData::view;
	using ImageData::cfger;
};

#endif // TEXT_H
