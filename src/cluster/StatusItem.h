#ifndef STATUSITEM_H
#define STATUSITEM_H

#include <QTableWidgetItem>

class StatusItem : public QTableWidgetItem
{
public:
	bool operator<(const QTableWidgetItem &other) const override;
};

#endif // STATUSITEM_H
