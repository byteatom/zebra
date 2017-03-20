#include "CalendarAttr.h"

#include "ISetting.h"
#include "UtilQt.h"

extern ISetting* setting;

static const QString refChar = '4';
static const QString refStr{refChar};

CalendarStyle::CalendarStyle(const QString& ui, const QString& format, const QString& frame):
	ui{ui}, format{format}, frame{frame}
{
}

bool CalendarStyle::year()
{
	return format.contains("yy");
}

bool CalendarStyle::longYear()
{
	return format.contains("yyyy");
}

bool CalendarStyle::month()
{
	return format.contains('M');
}

bool CalendarStyle::day()
{
	return format.contains('d');
}

bool CalendarStyle::hour()
{
	return format.contains('h', Qt::CaseInsensitive);
}

bool CalendarStyle::minute()
{
	return format.contains('m');
}

bool CalendarStyle::second()
{
	return format.contains('s');
}

DateStyle::DateStyle(const QString& ui, const QString& format, const QString& frame):
	CalendarStyle{ui, format, frame}
{
}

TimeStyle::TimeStyle(const QString& ui, const QString& format, const QString& frame):
	CalendarStyle{ui, format, frame}
{
}

DateStyle0::DateStyle0(): DateStyle{"YYYY/MM/DD", "yyyy/MM/dd", "    /  /  "}
{
}

void DateStyle0::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	year.setRect(box.x(), box.y(), refStrRect.width() * 4, refStrRect.height());
	month.setRect(year.x() + year.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	day.setRect(month.x() + month.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

DateStyle1::DateStyle1(): DateStyle{"MM/DD/YY", "MM/dd/yy", "  /  /  "}
{
}

void DateStyle1::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	month.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	day.setRect(month.x() + month.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	year.setRect(day.x() + day.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

DateStyle2::DateStyle2(): DateStyle{"DD/MM/YYYY", "dd/MM/yyyy", "  /  /    "}
{
}

void DateStyle2::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	day.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	month.setRect(day.x() + day.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	year.setRect(month.x() + month.width() + refStrRect.width(), box.y(), refStrRect.width() * 4, refStrRect.height());
}

DateStyle3::DateStyle3(): DateStyle{u8"YYYY年MM月DD日", u8"yyyy年MM月dd日", u8"    年  月  日"}
{
}

void DateStyle3::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& year, QRect& month, QRect& day)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	year.setRect(box.x(), box.y(), refStrRect.width() * 4, refStrRect.height());
	month.setRect(year.x() + year.width() + painter.boundingRect(box, flags, u8"年").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	day.setRect(month.x() + month.width() + painter.boundingRect(box, flags, u8"月").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

const std::unique_ptr<DateStyle> CalendarAttr::dateStyles[] = {
	std::make_unique<DateStyle0>(),
	std::make_unique<DateStyle1>(),
	std::make_unique<DateStyle2>(),
	std::make_unique<DateStyle3>(),
};

int CalendarAttr::dateStyleCount()
{
	return std::extent<decltype(dateStyles)>::value;
}

const QString CalendarAttr::weekStyles[][7] = {
	{u8"周一", u8"周二", u8"周三", u8"周四", u8"周五", u8"周六", u8"周日"},
	{u8"星期一", u8"星期二", u8"星期三", u8"星期四", u8"星期五", u8"星期六", u8"星期日"},
	{"Mon", "Tues", "Wed", "Thurs", "Fri", "Sat", "Sun"},
	{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"},
	{u8"يوم الإثنين", u8"يوم الثلاثاء", u8"يوم الأربعاء", u8"يوم الخميس", u8"يوم الجمعة", u8"يوم السبت", u8"يوم الأحد"},
};

int CalendarAttr::weekStyleCount()
{
	return std::extent<decltype(weekStyles)>::value;
}

TimeStyle0::TimeStyle0(): TimeStyle{"HH:MM:SS", "HH:mm:ss", "  :  :  "}
{
}

void TimeStyle0::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	hour.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	minute.setRect(hour.x() + hour.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	second.setRect(minute.x() + minute.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

TimeStyle1::TimeStyle1(): TimeStyle{"HH:MM", "HH:mm", "  :  "}
{
}

void TimeStyle1::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	hour.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	minute.setRect(hour.x() + hour.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	//second.setRect(minute.x() + minute.width() + refStrRect.width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

TimeStyle2::TimeStyle2(): TimeStyle{u8"HH时MM分SS秒", u8"HH时mm分ss秒", u8"  时  分  秒"}
{
}

void TimeStyle2::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	hour.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	minute.setRect(hour.x() + hour.width() + painter.boundingRect(box, flags, u8"时").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	second.setRect(minute.x() + minute.width() + painter.boundingRect(box, flags, u8"分").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

TimeStyle3::TimeStyle3(): TimeStyle{u8"HH时MM分", u8"HH时mm分", u8"  时  分"}
{
}

void TimeStyle3::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	hour.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	minute.setRect(hour.x() + hour.width() + painter.boundingRect(box, flags, u8"时").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	//second.setRect(minute.x() + minute.width() + painter.boundingRect(box, flags, u8"分").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

TimeStyle4::TimeStyle4(): TimeStyle{u8"HH時MM分SS秒", u8"HH時mm分ss秒", u8"  時  分  秒"}
{
}

void TimeStyle4::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	hour.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	minute.setRect(hour.x() + hour.width() + painter.boundingRect(box, flags, u8"時").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	second.setRect(minute.x() + minute.width() + painter.boundingRect(box, flags, u8"分").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

TimeStyle5::TimeStyle5(): TimeStyle{u8"HH時MM分", u8"HH時mm分", u8"  時  分"}
{
}

void TimeStyle5::getVarsRect(QPainter& painter, const QRect& box, int flags, QRect& hour, QRect& minute, QRect& second)
{
	QRect refStrRect = painter.boundingRect(box, flags, refStr);
	hour.setRect(box.x(), box.y(), refStrRect.width() * 2, refStrRect.height());
	minute.setRect(hour.x() + hour.width() + painter.boundingRect(box, flags, u8"時").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
	//second.setRect(minute.x() + minute.width() + painter.boundingRect(box, flags, u8"分").width(), box.y(), refStrRect.width() * 2, refStrRect.height());
}

const std::unique_ptr<TimeStyle> CalendarAttr::timeStyles[] = {
	std::make_unique<TimeStyle0>(),
	std::make_unique<TimeStyle1>(),
	std::make_unique<TimeStyle2>(),
	std::make_unique<TimeStyle3>(),
	std::make_unique<TimeStyle4>(),
	std::make_unique<TimeStyle5>(),
};

int CalendarAttr::timeStyleCount()
{
	return std::extent<decltype(timeStyles)>::value;
}

CalendarAttr::CalendarAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::CALENDAR, jnode}
{
	if (jnode) {
		jtov(multiLine);
		jtov(dateFirst);
		jtov(space);
		jtov(date);
		jtov(dateStyle);
		jtov(dateColor);
		jtov(week);
		jtov(weekStyle);
		jtov(weekColor);
		jtov(time);
		jtov(timeStyle);
		jtov(timeColor);
		jtov(offset);
		jtov(offsetHour);
		jtov(offsetMinute);
		jtov(family);
		jtov(point);
		jtov(bold);
		jtov(italic);
		jtov(underline);
		jtov(extraText);
		jtov(text);
		jtov(textColor);
	}
}

CalendarAttr::~CalendarAttr()
{
}

void CalendarAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(multiLine);
	vtoj(dateFirst);
	vtoj(space);
	vtoj(date);
	vtoj(dateStyle);
	vtoj(dateColor);
	vtoj(week);
	vtoj(weekStyle);
	vtoj(weekColor);
	vtoj(time);
	vtoj(timeStyle);
	vtoj(timeColor);
	vtoj(offset);
	vtoj(offsetHour);
	vtoj(offsetMinute);
	vtoj(family);
	vtoj(point);
	vtoj(bold);
	vtoj(italic);
	vtoj(underline);
	vtoj(extraText);
	vtoj(text);
	vtoj(textColor);
}

QString CalendarAttr::exportCoreJsonArea(Json& jArea)
{
	QRect rectContent = contentRect->rect();
	rectContent.moveTo(0, 0);

	QFont font{family,
				point,
				bold ? QFont::Bold : QFont::Normal,
				italic};
	font.setUnderline(underline);
	font.setStyleStrategy(QFont::NoAntialias);
	//QFontMetrics metrics{font};

	QPixmap framePix{rectContent.size()};
	framePix.fill(Qt::transparent);

	QPainter framePainter(&framePix);
	framePainter.setFont(font);
	framePainter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);

	int flags = 0;

	int spaceCount = 0;

	QString dateFrame;
	QRect dateFrameRect;
	if (date) {
		dateFrame = dateStyles[dateStyle]->frame;
		dateFrameRect = framePainter.boundingRect(rectContent, flags, dateFrame);
		spaceCount++;
	}
	QRect weekVarMaxRect;
	if (week) {
		for (int i = 0; i < 7; ++i) {
			QRect rect = framePainter.boundingRect(rectContent, 0, weekStyles[weekStyle][i]);
			if (rect.width() > weekVarMaxRect.width()) {
				weekVarMaxRect = rect;
			}
		}
		spaceCount++;
	}
	QString timeFrame;
	QRect timeFrameRect;
	if (time) {
		timeFrame = timeStyles[timeStyle]->frame;
		timeFrameRect = framePainter.boundingRect(rectContent, flags, timeFrame);
		spaceCount++;
	}
	QRect textRect;
	if (extraText && !text.isEmpty()) {
		textRect = framePainter.boundingRect(rectContent, 0, text);
		spaceCount++;
	}
	if (spaceCount > 0) spaceCount--;

	QRect yearVarRect;
	QRect monthVarRect;
	QRect dayVarRect;
	QRect hourVarRect;
	QRect minuteVarRect;
	QRect secondVarRect;

	if (multiLine) {
		int y = (rectContent.height() - textRect.height() - dateFrameRect.height()
				- weekVarMaxRect.height() - timeFrameRect.height() - spaceCount*space) / 2;

		if (extraText && !text.isEmpty()) {
			textRect.moveTo((rectContent.width() - textRect.width()) / 2, y);
			framePainter.setPen(QColor{textColor});
			framePainter.drawText(textRect, flags, text);
			y += (textRect.height() + space);
		}

		if (dateFirst) {
			if (date) {
				dateFrameRect.moveTo((rectContent.width() - dateFrameRect.width()) / 2, y);
				framePainter.setPen(QColor{dateColor});
				framePainter.drawText(dateFrameRect,  flags, dateFrame);
				dateStyles[dateStyle]->getVarsRect(framePainter, dateFrameRect, flags, yearVarRect, monthVarRect, dayVarRect);
				y += (dateFrameRect.height() + space);
			}

			if (week) {
				weekVarMaxRect.moveTo((rectContent.width() - weekVarMaxRect.width()) / 2, y);
				y += (weekVarMaxRect.height() + space);
			}
		} else {
			if (week) {
				weekVarMaxRect.moveTo((rectContent.width() - weekVarMaxRect.width()) / 2, y);
				y += (weekVarMaxRect.height() + space);
			}

			if (date) {
				dateFrameRect.moveTo((rectContent.width() - dateFrameRect.width()) / 2, y);
				framePainter.setPen(QColor{dateColor});
				framePainter.drawText(dateFrameRect,  flags, dateFrame);
				dateStyles[dateStyle]->getVarsRect(framePainter, dateFrameRect, flags, yearVarRect, monthVarRect, dayVarRect);
				y += (dateFrameRect.height() + space);
			}
		}

		if (time) {
			timeFrameRect.moveTo((rectContent.width() - timeFrameRect.width()) / 2, y);
			framePainter.setPen(QColor{timeColor});
			framePainter.drawText(timeFrameRect,  flags, timeFrame);
			timeStyles[timeStyle]->getVarsRect(framePainter, timeFrameRect, flags, hourVarRect, minuteVarRect, secondVarRect);
			y += (timeFrameRect.height() + space);
		}
	} else {
		int x = (rectContent.width() - textRect.width() - dateFrameRect.width()
				- weekVarMaxRect.width() - timeFrameRect.width() - spaceCount*space) / 2;

		if (extraText && !text.isEmpty()) {
			textRect.moveTo(x, (rectContent.height() - textRect.height()) / 2);
			framePainter.setPen(QColor{textColor});
			framePainter.drawText(textRect, flags, text);
			x += (textRect.width() + space);
		}

		if (dateFirst) {
			if (date) {
				dateFrameRect.moveTo(x, (rectContent.height() - dateFrameRect.height()) / 2);
				framePainter.setPen(QColor{dateColor});
				framePainter.drawText(dateFrameRect, flags, dateFrame);
				dateStyles[dateStyle]->getVarsRect(framePainter, dateFrameRect, flags, yearVarRect, monthVarRect, dayVarRect);
				x += (dateFrameRect.width() + space);
			}

			if (week) {
				weekVarMaxRect.moveTo(x, (rectContent.height() - weekVarMaxRect.height()) / 2);
				x += (weekVarMaxRect.width() + space);
			}
		} else {
			if (week) {
				weekVarMaxRect.moveTo(x, (rectContent.height() - weekVarMaxRect.height()) / 2);
				x += (weekVarMaxRect.width() + space);
			}

			if (date) {
				dateFrameRect.moveTo(x, (rectContent.height() - dateFrameRect.height()) / 2);
				framePainter.setPen(QColor{dateColor});
				framePainter.drawText(dateFrameRect, flags, dateFrame);
				dateStyles[dateStyle]->getVarsRect(framePainter, dateFrameRect, flags, yearVarRect, monthVarRect, dayVarRect);
				x += (dateFrameRect.width() + space);
			}
		}

		if (time) {
			timeFrameRect.moveTo(x, (rectContent.height() - timeFrameRect.height()) / 2);
			framePainter.setPen(QColor{timeColor});
			framePainter.drawText(timeFrameRect, flags, timeFrame);
			timeStyles[timeStyle]->getVarsRect(framePainter, timeFrameRect, flags, hourVarRect, minuteVarRect, secondVarRect);
			x += (timeFrameRect.width() + space);
		}
	}

	QString backgroundPath = UtilQt::uuidFile(setting->tempDir(), ".bmp");
	framePix.save(backgroundPath);

	Json dateCharsJson;
	Json timeCharsJson;
	QRect refStrRect = framePainter.boundingRect(rectContent, flags, refStr);
	static const QString Digits[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "P", "M"};
	for (int i = 0; i < std::extent<decltype(Digits)>::value; ++i) {
		QPixmap timeCharPix{refStrRect.size()};
		timeCharPix.fill(Qt::transparent);
		QPainter timeCharPainter(&timeCharPix);
		timeCharPainter.setFont(font);
		timeCharPainter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
		timeCharPainter.setPen(QColor{timeColor});
		timeCharPainter.drawText(refStrRect, flags, Digits[i]);
		QString timeCharPath = UtilQt::uuidFile(setting->tempDir(), ".bmp");
		timeCharPix.save(timeCharPath);
		timeCharsJson.push_back(timeCharPath);

		if (i > 9) continue;

		QPixmap dateCharPix{refStrRect.size()};
		dateCharPix.fill(Qt::transparent);
		QPainter dateCharPainter(&dateCharPix);
		dateCharPainter.setFont(font);
		dateCharPainter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
		dateCharPainter.setPen(QColor{dateColor});
		dateCharPainter.drawText(refStrRect, flags, Digits[i]);
		QString dateCharPath = UtilQt::uuidFile(setting->tempDir(), ".bmp");
		dateCharPix.save(dateCharPath);
		dateCharsJson.push_back(dateCharPath);
	}

	Json weekVarsJson;
	QRect weekVarRect = weekVarMaxRect;
	weekVarRect.moveTo(0, 0);
	for (int i = 0; i < 7; ++i) {
		QString weekVar = weekStyles[weekStyle][i];
		QPixmap weekVarPix{weekVarRect.size()};
		weekVarPix.fill(Qt::transparent);
		QPainter weekVarPainter(&weekVarPix);
		weekVarPainter.setFont(font);
		weekVarPainter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
		weekVarPainter.setPen(QColor{weekColor});
		weekVarPainter.drawText(weekVarRect, flags | Qt::AlignHCenter, weekVar);
		QString weekVarPath = UtilQt::uuidFile(setting->tempDir(), ".bmp");
		weekVarPix.save(weekVarPath);
		weekVarsJson[i == 6 ? 0 : i + 1] = weekVarPath;
	}

	QPoint lowYearPoint = (!dateStyles[dateStyle]->longYear() ? yearVarRect.topLeft()
										  : yearVarRect.topLeft() + QPoint{refStrRect.width() * 2, 0});
	jArea = {
		{"startX", 0},
		{"startY", 0},
		{"jetLag", QString("%1:%2:0").arg(offset == 0 ? offsetHour : -offsetHour).arg(offset == 0 ? offsetMinute : -offsetMinute)},
		{"backgroundContent", backgroundPath},
		{"showYearH", date && dateStyles[dateStyle]->longYear()},
		{"showYearL", date && dateStyles[dateStyle]->year()},
		{"showMonth", date && dateStyles[dateStyle]->month()},
		{"showDay", date && dateStyles[dateStyle]->day()},
		{"showWeek", week},
		{"showHour", time && timeStyles[timeStyle]->hour()},
		{"showMin", time && timeStyles[timeStyle]->minute()},
		{"showSec", time && timeStyles[timeStyle]->second()},
		{"showAPM", false},
		{"timeMode", "24Hours"},
		{"dateFontWidth", refStrRect.width()},
		{"dateFontHeight", refStrRect.height()},
		{"datePictureWidth", refStrRect.width()},
		{"datePictureHeight", refStrRect.height()},
		{"dateYearHX", yearVarRect.x()},
		{"dateYearHY", yearVarRect.y()},
		{"dateYearLX", lowYearPoint.x()},
		{"dateYearLY", lowYearPoint.y()},
		{"dateMonthX", monthVarRect.x()},
		{"dateMonthY", monthVarRect.y()},
		{"dateDayX", dayVarRect.x()},
		{"dateDayY", dayVarRect.y()},
		{"dateContentArray", dateCharsJson},
		{"weekFontWidth", weekVarMaxRect.width()},
		{"weekFontHeight", weekVarMaxRect.height()},
		{"weekPictureWidth", weekVarMaxRect.width()},
		{"weekPictureHeight", weekVarMaxRect.height()},
		{"weekX", weekVarMaxRect.x()},
		{"weekY", weekVarMaxRect.y()},
		{"weekContentArray", weekVarsJson},
		{"timeFontWidth", refStrRect.width()},
		{"timeFontHeight", refStrRect.height()},
		{"timePictureWidth", refStrRect.width()},
		{"timePictureHeight", refStrRect.height()},
		{"timeHourX", hourVarRect.x()},
		{"timeHourY", hourVarRect.y()},
		{"timeMinX", minuteVarRect.x()},
		{"timeMinY", minuteVarRect.y()},
		{"timeSecX", secondVarRect.x()},
		{"timeSecY", secondVarRect.y()},
		{"timeContentArray", timeCharsJson},
	};

	return QString();
}

