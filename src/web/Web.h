#ifndef WEB_H
#define WEB_H

#include "WebView.h"
#include "WebCfger.h"
#include "WebAttr.h"
#include "Region.h"

struct WebData
{
	explicit WebData(INodeBox* box, Json* jnode);
	~WebData();

	WebAttr* attr;
	WebView* view;
	WebCfger* cfger;
};

class Web : private WebData, public Region
{
public:
	explicit Web(INodeBox* box, Json* jnode);

private:
	virtual ~Web() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void stop() override;

	virtual boost::signals2::connection pageObserve(PageObserver observer) override;
	virtual void pageFirst() override;
	virtual void pagePrevious() override;
	virtual void pageNext() override;
	virtual void pageLast() override;

	using WebData::attr;
	using WebData::view;
	using WebData::cfger;
};

#endif // WEB_H
