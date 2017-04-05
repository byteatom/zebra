#ifndef WEBITEM_H
#define WEBITEM_H

#include <QIcon>
#include <QUrlQuery>

#include <vector>

#include "Json.h"

class WebItem
{
public:
	typedef std::vector<WebItem> List;
	static const int thumbWidth = 200;
	static const int thumbHeight = 150;
	static List origin();

	explicit WebItem(Json* jnode);
	explicit WebItem(const QString& dir);
	virtual ~WebItem() = default;

	virtual void exportProjJson(Json& jnode) const;
	QIcon thumb() const;
	QUrl url() const;
	QUrl uri() const;
	Json queryInfo() const;

	QString dir;	//relative
	QUrlQuery query;

	int duration = 10000;
};

typedef WebItem::List WebItems;

#endif // WEBITEM_H
