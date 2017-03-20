#ifndef CALENDARATTRCFGER_H
#define CALENDARATTRCFGER_H

#include <QGroupBox>
#include <QButtonGroup>

#include "CalendarAttr.h"

namespace Ui {
class CalendarAttrCfger;
}

class CalendarAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit CalendarAttrCfger(QWidget *parent, CalendarAttr *attr);
	~CalendarAttrCfger();

	void textChanged();
private:
	Ui::CalendarAttrCfger* ui;
	QButtonGroup mode;
	CalendarAttr* attr = nullptr;
};

#endif // CALENDARATTRCFGER_H
