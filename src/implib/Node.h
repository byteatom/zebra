#ifndef NODE_H
#define NODE_H

#include <QCoreApplication>

#include "INode.h"
#include "INodeBox.h"
#include "NodeAttr.h"
#include "View.h"
#include "Cfger.h"

class Node : public INode
{
protected:
	explicit Node(NodeAttr* attr, View* view, Cfger* cfger):
		attr{attr}, view_{view}, cfger_{cfger}
	{
		attr->box->append(this);
	}

	virtual ~Node()
	{
		attr->box->remove(this);
		if (data_) {
			delete data_;
			data_ = nullptr;
		}
	}

	virtual INodeBox* box() override
	{
		return attr->box;
	}

	virtual Type type() override
	{
		return static_cast<Type>(attr->type);
	}

	virtual QString typeName() override
	{
		return attr->typeName();
	}

	virtual QString name() override
	{
		return attr->name;
	}

	virtual void setName(QString& name) override
	{
		attr->name = name;
	}

	virtual INodeData* data() override
	{
		return data_;
	}

	virtual void setData(INodeData* data) override
	{
		data_ = data;
	}

	virtual QWidget* view() override
	{
		return view_;
	}

	virtual QWidget* cfger() override
	{
		return cfger_;
	}

	virtual void activate() override
	{
		attr->active = true;
		attr->box->activateView(view_);
		attr->box->activateCfger(cfger_);
		view_->activate();
	}

	virtual void deactive() override
	{
		attr->active = false;
		view_->deactive();
	}

	virtual void userDelete() override
	{
	}

private:
	NodeAttr* attr = nullptr;
	View* view_ = nullptr;
	Cfger* cfger_ = nullptr;
	INodeData* data_ = nullptr;
};

#endif // NODE_H
