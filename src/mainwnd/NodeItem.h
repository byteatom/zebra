#ifndef NODEITEM_H
#define NODEITEM_H

#include "INode.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>

class NodeItem : public QTreeWidgetItem
{
public:
	NodeItem(QTreeWidget* parent, const QStr& title, INode* node);
	NodeItem(QTreeWidgetItem* parent, const QStr& title, INode* node);
	virtual ~NodeItem();

	void setIcon();

	INode* node = nullptr;
};

class NodeData : public INodeData
{
public:
	explicit NodeData(QTreeWidgetItem* item);
	virtual ~NodeData();

	virtual void activate() override;

	QTreeWidgetItem* item = nullptr;
};

#endif // NODEITEM_H
