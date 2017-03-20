#ifndef CLOCKATTRCFGER_H
#define CLOCKATTRCFGER_H

#include <QGroupBox>

#include "ClockAttr.h"

namespace Ui {
class ClockAttrCfger;
}

class ClockAttrCfger : public QGroupBox
{
	Q_OBJECT
public:
	explicit ClockAttrCfger(QWidget *parent, ClockAttr *attr);
	~ClockAttrCfger();

private slots:
	void dialStyle(int index);

	void hourPointSize(int);
	void hourPointColor(int);
	void tsnPointSize(int);
	void tsnPointColor(int);
	void tsnPointStyle(const QString &);
	void minutePointSize(int);
	void minutePointColor(int);

	void hourHandSize(int);
	void hourHandColor(int);
	void hourHandStyle(const QString &);
	void minuteHandSize(int);
	void minuteHandColor(int);
	void minuteHandStyle(const QString &);
	void secondHandSize(int);
	void secondHandColor(int);
	void secondHandStyle(const QString &);

	void deltType(const QString &);
	void deltTime(const QTime &time);

private:
	void init(ClockAttr *attr);

	ClockAttr* attr = nullptr;
	Ui::ClockAttrCfger* ui;
};

#endif // CLOCKATTRCFGER_H
