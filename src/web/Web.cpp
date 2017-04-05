#include "Web.h"

WebData::WebData(INodeBox* box, Json* jnode)
{
	attr = new WebAttr{box, jnode},
	view = new WebView{attr},
	cfger = new WebCfger{attr};
}

WebData::~WebData()
{
	delete cfger;
	delete view;
	delete attr;
}

Web::Web(INodeBox *box, Json* jnode):
	WebData{box, jnode},
	Region{attr, view, cfger}
{

}

void Web::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Web::play()
{
	Region::play();
}

void Web::stop()
{
	Region::stop();
}

boost::signals2::connection Web::pageObserve(PageObserver observer)
{
	return attr->pageObserve(observer);
}

void Web::pageFirst()
{
	return attr->pageFirst();
}

void Web::pagePrevious()
{
	return attr->pagePrevious();
}

void Web::pageNext()
{
	return attr->pageNext();
}

void Web::pageLast()
{
	return attr->pageLast();
}
