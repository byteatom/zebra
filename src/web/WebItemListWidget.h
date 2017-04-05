#ifndef WEBITEMLISTWIDGET_H
#define WEBITEMLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>

#include "WebItem.h"

class WebItemListWidget : public QListWidget
{
	Q_OBJECT
public:
	explicit WebItemListWidget(QWidget *parent = Q_NULLPTR);
	virtual ~WebItemListWidget();

signals:
	void itemNull();

private:
	virtual void mouseMoveEvent(QMouseEvent *event) override;
};

class WebItemListWidgetItem : public QListWidgetItem
{
public:
	WebItemListWidgetItem(const WebItem& item, WebItemListWidget* parent);
	WebItemListWidgetItem(const WebItemListWidgetItem& other);
	~WebItemListWidgetItem();

	WebItem item;
};

#endif // WEBITEMLISTWIDGET_H
