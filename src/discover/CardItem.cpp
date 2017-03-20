#include "CardItem.h"

CardItem::CardItem(const QString &text, CardInfo* card):
	QTableWidgetItem{text},
	card{card}
{

}

CardItem::~CardItem()
{
	delete card;
}
