#include "ClockAttrCfger.h"
#include "ui_ClockAttrCfger.h"

#include <functional>

#include <QColorDialog>
#include <QFontMetrics>
#include <QFont>
#include <QDir>

#define WATCH_RANGE_MIN 0
#define WATCH_RANGE_MAX 9
#define DOT_SIZE_MIN WATCH_RANGE_MIN
#define DOT_SIZE_MAX WATCH_RANGE_MAX
#define HAND_SIZE_MIN WATCH_RANGE_MIN
#define HAND_SIZE_MAX WATCH_RANGE_MAX

ClockAttrCfger::ClockAttrCfger(QWidget *parent, ClockAttr *attr) :
	QGroupBox(parent),
	ui(new Ui::ClockAttrCfger),
	attr{attr}
{
	ui->setupUi(this);

	QStringList dialStyleList;
    dialStyleList << ClockAttrCfger::tr("Circle") << ClockAttrCfger::tr("Ellipse")
                  << ClockAttrCfger::tr("Rhombus") << ClockAttrCfger::tr("RoundRect");
	ui->dialStyle->addItems(dialStyleList);
	ui->dialStyle->setCurrentIndex(0);

	ui->hourPointSize->setRange(DOT_SIZE_MIN, DOT_SIZE_MAX);
	ui->tsnPointSize->setRange(DOT_SIZE_MIN, DOT_SIZE_MAX);
	ui->minutePointSize->setRange(DOT_SIZE_MIN, DOT_SIZE_MAX);
	QStringList pointStyleList;
    pointStyleList << ClockAttrCfger::tr("Circle") << ClockAttrCfger::tr("Line");
	ui->tsnPointStyle->addItems(pointStyleList);
	ui->tsnPointStyle->setCurrentIndex(0);
	ui->hourHandSize->setRange(HAND_SIZE_MIN, HAND_SIZE_MAX);
	ui->minuteHandSize->setRange(HAND_SIZE_MIN, HAND_SIZE_MAX);
	ui->secondHandSize->setRange(HAND_SIZE_MIN, HAND_SIZE_MAX);
	QStringList handStyleList;
    handStyleList << ClockAttrCfger::tr("Rhombus") << ClockAttrCfger::tr("Line");
	ui->hourHandStyle->addItems(handStyleList);
	ui->hourHandStyle->setCurrentIndex(0);
	ui->minuteHandStyle->addItems(handStyleList);
	ui->minuteHandStyle->setCurrentIndex(0);
	ui->secondHandStyle->addItems(handStyleList);
	ui->secondHandStyle->setCurrentIndex(0);
	QStringList deltTypeList;
    deltTypeList << "+" << "-";
	ui->deltType->addItems(deltTypeList);
	ui->deltType->setCurrentIndex(0);
	ui->deltTime->setTime(QTime());

	Attr<int>& colorMode = attr->box->colorMode();
	ui->hourPointColor->setMode(colorMode);
	ui->tsnPointColor->setMode(colorMode);
	ui->minutePointColor->setMode(colorMode);
	ui->hourHandColor->setMode(colorMode);
	ui->minuteHandColor->setMode(colorMode);
	ui->secondHandColor->setMode(colorMode);

	init(attr);

	connect(ui->dialStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(dialStyle(int)));

	connect(ui->hourPointSize, SIGNAL(valueChanged(int)), this, SLOT(hourPointSize(int)));
	connect(ui->hourPointColor, SIGNAL(currentIndexChanged(int)), this, SLOT(hourPointColor(int)));
	connect(ui->tsnPointSize, SIGNAL(valueChanged(int)), this, SLOT(tsnPointSize(int)));
	connect(ui->tsnPointColor, SIGNAL(currentIndexChanged(int)), this, SLOT(tsnPointColor(int)));
	connect(ui->tsnPointStyle, SIGNAL(currentTextChanged(const QString &)), this,
			SLOT(tsnPointStyle(const QString &)));
	connect(ui->minutePointSize, SIGNAL(valueChanged(int)), this, SLOT(minutePointSize(int)));
	connect(ui->minutePointColor, SIGNAL(currentIndexChanged(int)), this, SLOT(minutePointColor(int)));
	connect(ui->hourHandSize, SIGNAL(valueChanged(int)), this, SLOT(hourHandSize(int)));
	connect(ui->hourHandColor, SIGNAL(currentIndexChanged(int)), this, SLOT(hourHandColor(int)));
	connect(ui->hourHandStyle, SIGNAL(currentTextChanged(const QString &)), this,
			SLOT(hourHandStyle(const QString &)));
	connect(ui->minuteHandSize, SIGNAL(valueChanged(int)), this, SLOT(minuteHandSize(int)));
	connect(ui->minuteHandColor, SIGNAL(currentIndexChanged(int)), this, SLOT(minuteHandColor(int)));
	connect(ui->minuteHandStyle, SIGNAL(currentTextChanged(const QString &)), this,
			SLOT(minuteHandStyle(const QString &)));
	connect(ui->secondHandSize, SIGNAL(valueChanged(int)), this, SLOT(secondHandSize(int)));
	connect(ui->secondHandColor, SIGNAL(currentIndexChanged(int)), this, SLOT(secondHandColor(int)));
	connect(ui->secondHandStyle, SIGNAL(currentTextChanged(const QString &)), this,
			SLOT(secondHandStyle(const QString &)));
	connect(ui->deltType, SIGNAL(currentTextChanged(const QString &)), this,
			SLOT(deltType(const QString &)));
	connect(ui->deltTime, SIGNAL(timeChanged(QTime)), this, SLOT(deltTime(QTime)));

	attr->changed();
}

ClockAttrCfger::~ClockAttrCfger()
{
	delete ui;
}

void ClockAttrCfger::init(ClockAttr *attr)
{
	ui->dialStyle->setCurrentIndex(static_cast<int>(attr->dialAttrInfo.dialStyle));

	ui->hourPointColor->setCurrentIndex(attr->dialAttrInfo.dialTime.dialPoint.hourPoint.pointColor);
	ui->tsnPointColor->setCurrentIndex(attr->dialAttrInfo.dialTime.dialPoint.tsnPoint.pointColor);
	ui->minutePointColor->setCurrentIndex(attr->dialAttrInfo.dialTime.dialPoint.minutePoint.pointColor);
	ui->hourHandColor->setCurrentIndex(attr->dialAttrInfo.dialTime.dialHand.hourHand.handColor);
	ui->minuteHandColor->setCurrentIndex(attr->dialAttrInfo.dialTime.dialHand.minuteHand.handColor);
	ui->secondHandColor->setCurrentIndex(attr->dialAttrInfo.dialTime.dialHand.secondHand.handColor);

	ui->hourPointSize->setValue(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialPoint.hourPoint.pointSize));
	ui->tsnPointSize->setValue(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialPoint.tsnPoint.pointSize));
	ui->tsnPointStyle->setCurrentIndex(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialPoint.tsnPoint.pointStyle));
	ui->minutePointSize->setValue(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialPoint.minutePoint.pointSize));
	ui->hourHandSize->setValue(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialHand.hourHand.handSize));
	ui->hourHandStyle->setCurrentIndex(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialHand.hourHand.handStyle));
	ui->minuteHandSize->setValue(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialHand.minuteHand.handSize));
	ui->minuteHandStyle->setCurrentIndex(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialHand.minuteHand.handStyle));
	ui->secondHandSize->setValue(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialHand.secondHand.handSize));
	ui->secondHandStyle->setCurrentIndex(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialHand.secondHand.handStyle));
	ui->deltType->setCurrentIndex(
		static_cast<int>(attr->dialAttrInfo.dialTime.dialTimeDiff.deltType));
	QTime deltTime(attr->dialAttrInfo.dialTime.dialTimeDiff.deltHour,
				   attr->dialAttrInfo.dialTime.dialTimeDiff.deltMinute,
				   attr->dialAttrInfo.dialTime.dialTimeDiff.deltSecond);
	ui->deltTime->setTime(deltTime);

}


void ClockAttrCfger::dialStyle(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialStyle = static_cast<DialStyle>(ui->dialStyle->currentIndex());
		attr->changed();
	}
}

void ClockAttrCfger::hourPointSize(int)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.hourPoint.pointSize =
			ui->hourPointSize->value();
		attr->changed();
	}
}

void ClockAttrCfger::hourPointColor(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.hourPoint.pointColor = index;
		attr->changed();
	}
}

void ClockAttrCfger::tsnPointSize(int)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.tsnPoint.pointSize = ui->tsnPointSize->value();
		attr->changed();
	}
}

void ClockAttrCfger::tsnPointColor(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.tsnPoint.pointColor = index;
		attr->changed();
	}
}

void ClockAttrCfger::tsnPointStyle(const QString &style)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.tsnPoint.pointStyle =
			static_cast<DialPointStyle>(ui->tsnPointStyle->currentIndex());
		attr->dialAttrInfo.dialTime.dialPoint.hourPoint.pointStyle =
			static_cast<DialPointStyle>(ui->tsnPointStyle->currentIndex());
		attr->dialAttrInfo.dialTime.dialPoint.minutePoint.pointStyle =
			static_cast<DialPointStyle>(ui->tsnPointStyle->currentIndex());
		attr->changed();
	}
}

void ClockAttrCfger::minutePointSize(int)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.minutePoint.pointSize =
			ui->minutePointSize->value();
		attr->changed();
	}
}

void ClockAttrCfger::minutePointColor(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialPoint.minutePoint.pointColor = index;
		attr->changed();
	}
}

void ClockAttrCfger::hourHandSize(int)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.hourHand.handSize = ui->hourHandSize->value();
		attr->changed();
	}
}

void ClockAttrCfger::hourHandColor(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.hourHand.handColor = index;
		attr->changed();
	}
}

void ClockAttrCfger::hourHandStyle(const QString &)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.hourHand.handStyle =
			static_cast<DialHandStyle>(ui->hourHandStyle->currentIndex());
		attr->changed();
	}
}

void ClockAttrCfger::minuteHandSize(int)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.minuteHand.handSize =
			ui->minuteHandSize->value();
		attr->changed();
	}
}

void ClockAttrCfger::minuteHandColor(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.minuteHand.handColor = index;
		attr->changed();
	}
}

void ClockAttrCfger::minuteHandStyle(const QString &)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.minuteHand.handStyle =
			static_cast<DialHandStyle>(ui->minuteHandStyle->currentIndex());
		attr->changed();
	}
}

void ClockAttrCfger::secondHandSize(int)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.secondHand.handSize =
			ui->secondHandSize->value();
		attr->changed();
	}
}

void ClockAttrCfger::secondHandColor(int index)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.secondHand.handColor = index;
		attr->changed();
	}
}

void ClockAttrCfger::secondHandStyle(const QString &)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialHand.secondHand.handStyle =
			static_cast<DialHandStyle>(ui->secondHandStyle->currentIndex());
		attr->changed();
	}
}

void ClockAttrCfger::deltType(const QString &)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialTimeDiff.deltType =
			static_cast<DialDeltType>(ui->deltType->currentIndex());
		attr->changed();
	}
}

void ClockAttrCfger::deltTime(const QTime &time)
{
	if (attr) {
		attr->dialAttrInfo.dialTime.dialTimeDiff.deltHour = time.hour();
		attr->dialAttrInfo.dialTime.dialTimeDiff.deltMinute = time.minute();
		attr->dialAttrInfo.dialTime.dialTimeDiff.deltSecond = time.second();
		attr->changed();
	}
}
