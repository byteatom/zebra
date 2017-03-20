#ifndef CALENDARATTR_H
#define CALENDARATTR_H

#include <memory>

#include <boost/signals2.hpp>

#include <QPainter>

#include "RegionAttr.h"

struct CalendarStyle
{
	CalendarStyle(const QString& ui, const QString& format, const QString& frame);
	QString ui;
	QString format;
	QString frame;
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& yh, QRect& mm, QRect& ds) = 0;
	bool year();
	bool longYear();
	bool month();
	bool day();
	bool hour();
	bool minute();
	bool second();
};

struct DateStyle : public CalendarStyle
{
	DateStyle(const QString& ui, const QString& format, const QString& frame);
};

struct DateStyle0 : public DateStyle
{
	DateStyle0();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day) override;
};

struct DateStyle1 : public DateStyle
{
	DateStyle1();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day) override;
};

struct DateStyle2 : public DateStyle
{
	DateStyle2();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day) override;
};

struct DateStyle3 : public DateStyle
{
	DateStyle3();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day) override;
};

struct TimeStyle : public CalendarStyle
{
	TimeStyle(const QString& ui, const QString& format, const QString& frame);
};

struct TimeStyle0 : public TimeStyle
{
	TimeStyle0();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second) override;
};

struct TimeStyle1 : public TimeStyle
{
	TimeStyle1();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second) override;
};

struct TimeStyle2 : public TimeStyle
{
	TimeStyle2();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second) override;
};

struct TimeStyle3 : public TimeStyle
{
	TimeStyle3();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second) override;
};

struct TimeStyle4 : public TimeStyle
{
	TimeStyle4();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second) override;
};

struct TimeStyle5 : public TimeStyle
{
	TimeStyle5();
	virtual void getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second) override;
};

class CalendarAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(CalendarAttr)
public:
	static const std::unique_ptr<DateStyle> dateStyles[];
	static int dateStyleCount();
	static const QString weekStyles[][7];
	static int weekStyleCount();
	static const std::unique_ptr<TimeStyle> timeStyles[];
	static int timeStyleCount();

	explicit CalendarAttr(INodeBox* box, Json* jnode);
	virtual ~CalendarAttr();

	bool multiLine = true;
	bool dateFirst = true;
	int space = 0;
	bool date = true;
	int dateStyle = 0;
	QRgb dateColor = qRgba(255, 0, 0, 255);
	bool week = true;
	int weekStyle = 0;
	QRgb weekColor = qRgba(255, 0, 0, 255);
	bool time = true;
	int timeStyle = 0;
	QRgb timeColor = qRgba(255, 0, 0, 255);
	int offset = 0;
	int offsetHour = 0;
	int offsetMinute = 0;
	bool extraText = false;
	QStr text;
	QRgb textColor = qRgba(255, 0, 0, 255);
	QStr family;
	int point = 10;
	bool bold = false;
	bool italic = false;
	bool underline = false;

	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // CALENDARATTR_H
