#include "NodeItem.h"

NodeItem::NodeItem(QTreeWidget* parent, const QStr& title, INode* node):
	QTreeWidgetItem{parent, {{title}}},
	node{node}
{
	setIcon();
	setFlags(flags() | Qt::ItemIsEditable);
}

NodeItem::NodeItem(QTreeWidgetItem* parent, const QStr& title, INode* node):
	QTreeWidgetItem{parent, {{title}}},
	node{node}
{
	setIcon();
	setFlags(flags() | Qt::ItemIsEditable);
}

NodeItem::~NodeItem()
{
	node = nullptr;
}

void NodeItem::setIcon()
{
	static const QString iconPaths[] = {
		"image/icon/screen_16.png",
		"image/icon/program_64.png",
		"image/icon/text_64.png",
		"image/icon/image_64.png",
		"image/icon/clock_64.png",
		"image/icon/calendar_64.png",
		"image/icon/timer_64.png",
		"image/icon/flash_text_64.png",
		"image/icon/humiture_48.png",
		"image/icon/variable_64.png"
	};
	QTreeWidgetItem::setIcon(0, QIcon{iconPaths[node->type()]});
}

NodeData::NodeData(QTreeWidgetItem* item):
	item{item}
{
}

NodeData::~NodeData()
{
	if (item) {
		delete item;
		item = nullptr;
	}
}

void NodeData::activate()
{
	item->treeWidget()->setCurrentItem(item);
}

