#ifndef CARDITEM_H
#define CARDITEM_H

#include <QTableWidgetItem>
#include <QHostAddress>

#include "message.h"
#include "ICarrier.h"

struct CardInfo : public InfoGetRspData
{
	CardInfo(QByteArray &data): InfoGetRspData(data) {}

	ICarrier::Type type;
	QString name;
	QString intf;
	QString port;
	QString info;
	QHostAddress local;
	QHostAddress remote;
	QHostAddress netmask;
	QHostAddress broadcast;
};

class CardItem : public QTableWidgetItem
{
public:
	explicit CardItem(const QString &text, CardInfo* card);
	virtual ~CardItem();

	CardInfo* card;
};

#endif // CARDITEM_H
