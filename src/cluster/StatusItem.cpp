#include "StatusItem.h"

bool StatusItem::operator<(const QTableWidgetItem &other) const
{
	return text().toInt() < other.text().toInt();
}
