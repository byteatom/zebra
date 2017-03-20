#ifndef NODEBOX_H
#define NODEBOX_H

#include <list>

#include "INodeBox.h"
#include "INode.h"
#include "Util.h"

typedef std::list<INode *> Nodes;
typedef Nodes::iterator NodeIter;

class NodeBox : public INodeBox
{
protected:
	virtual ~NodeBox()
	{
		clear();
	}

	virtual void append(INode *child) override
	{
		children.push_back(child);
	}

	virtual void remove(INode *child) override
	{
		children.remove(child);
	}

	virtual void raise(INode* child) override
	{
		Util::raiseElement(children, child);
	}

	virtual void lower(INode* child) override
	{
		Util::lowerElement(children, child);
	}

	virtual INode* next(std::shared_ptr<void>& pos) override
	{
		if (!pos) {
			NodeIter it{children.begin()};
			if (it == children.end())
				return nullptr;
			else {
				pos = std::shared_ptr<void>(new NodeIter{it});
				return *it;
			}
		} else {
			NodeIter& it = *static_cast<NodeIter*>(pos.get());
			++it;
			if (it == children.end())
				return nullptr;
			else {
				return *it;
			}
		}
	}

	virtual void clear() override
	{
		auto iter = children.begin();
		while (iter != children.end()) {
			INode *child = *iter;
			iter = children.erase(iter);
			delete child;
		}
	}

	virtual QString buildName(const QString& prefix) override
	{
		QString name;
		int i = 0;
		do {
			++i;
			name = prefix + " " + QString::number(i);
		} while (!checkName(nullptr, name));
		return name;
	}

	virtual bool checkName(INode* child, const QString& name) override
	{
		for (INode* node: children) {
			if (node == child) continue;
			if (node->name() == name) return false;
		}
		return true;
	}

	virtual void activateCfger(QWidget* cfger) override
	{
		INode* node = dynamic_cast<INode*>(this);
		if (node) node->box()->activateCfger(cfger);
	}

	virtual Attr<int>& colorMode() override
	{
		INode* node = dynamic_cast<INode*>(this);
		if (node)
			return node->box()->colorMode();
		else
			return *(Attr<int>*)(nullptr);
	}

	virtual Attr<KRect>& contentRect() override
	{
		return *(Attr<KRect>*)(nullptr);
	}

protected:
	Nodes children;
};

#endif // NODEBOX_H
