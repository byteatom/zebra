#ifndef TIMERATTRCFGER_H
#define TIMERATTRCFGER_H

#include <QGroupBox>
#include <QButtonGroup>
#include <QCheckBox>

#include "TimerAttr.h"

namespace Ui {
class TimerAttrCfger;
}

class TimerAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit TimerAttrCfger(QWidget *parent, TimerAttr *attr);
	~TimerAttrCfger();

private:
	Ui::TimerAttrCfger* ui;
	QButtonGroup mode;
	TimerAttr* attr = nullptr;
	QCheckBox* parts[4];

	void makeContinuity(QCheckBox* current);
};

#endif // TIMERATTRCFGER_H
