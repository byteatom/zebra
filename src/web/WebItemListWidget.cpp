#include "WebItemListWidget.h"

WebItemListWidget::WebItemListWidget(QWidget *parent):
	QListWidget{parent}
{
	setMouseTracking(true);
}

WebItemListWidget::~WebItemListWidget()
{
}

void WebItemListWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!itemAt(event->pos())) emit itemNull();
	QListWidget::mouseMoveEvent(event);
}

WebItemListWidgetItem::WebItemListWidgetItem(const WebItem& item, WebItemListWidget* parent)
	:QListWidgetItem{item.thumb(), nullptr, parent, Type},
	  item{item}
{
}

WebItemListWidgetItem::WebItemListWidgetItem(const WebItemListWidgetItem& other)
	:QListWidgetItem{other},
	  item{other.item}
{

}

WebItemListWidgetItem::~WebItemListWidgetItem()
{
}
