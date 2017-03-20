#ifndef REGION_H
#define REGION_H

#include "Node.h"
#include "IRegion.h"
#include "RegionAttr.h"
#include "RegionView.h"
#include "RegionCfger.h"

class Region : public Node, public IRegion
{
protected:
	explicit Region(RegionAttr* attr, RegionView* view, RegionCfger* cfger):
		Node{this->attr = attr, this->view = view, this->cfger = cfger}
	{
	}

	virtual ~Region() = default;

	virtual void setData(INodeData* data) override
	{
		Node::setData(data);
		view->data = data;
	}

	virtual void zoom(qreal factor) override
	{
		view->zoomFactor = factor;
		view->setGeometry();
	}

	virtual void play() override
	{
		playing = true;
		view->play();
	}

	virtual void draw() override
	{
		view->drawAll();
	}

	virtual void stop() override
	{
		playing = false;
		view->stop();
	}

	virtual boost::signals2::connection pageObserve(PageObserver observer) override
	{
		observer(1, 0);
		return boost::signals2::connection();
	}

	virtual void pageFirst() override
	{
		return;
	}

	virtual void pagePrevious() override
	{
		return;
	}

	virtual void pageNext() override
	{
		return;
	}

	virtual void pageLast() override
	{
		return;
	}

	virtual Attr<KRect>& geometry() override
	{
		return attr->geometry;
	}

	virtual Attr<KRect>& contentRect() override
	{
		return attr->contentRect;
	}

	virtual QString exportCoreJson(Json& jnodes) override
	{
		return attr->exportCoreJson(jnodes);
	}

	RegionAttr* attr;
	RegionView* view;
	RegionCfger* cfger;
	bool playing = false;
};

#endif // REGION_H
