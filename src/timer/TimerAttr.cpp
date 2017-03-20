#include "TimerAttr.h"

#include <QPainter>

#include "ISetting.h"
#include "UtilQt.h"

extern ISetting* setting;
static const QString refChar = '4';
static const QString refStr{refChar};

TimerAttr::TimerAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::TIMER, jnode}
{
	if (jnode) {
		jtov(elapse);
		if (jnode->find("target") != jnode->end())
			target = QDateTime::fromString((*jnode)["target"], Qt::ISODate);
		jtov(day);
		jtov(hour);
		jtov(minute);
		jtov(second);
		jtov(unit);
		jtov(unitColor);
		jtov(digitColor);
		jtov(family);
		jtov(point);
		jtov(bold);
		jtov(italic);
		jtov(underline);
		jtov(extraText);
		jtov(text);
		jtov(extraLine);
		jtov(textColor);
	}
}

TimerAttr::~TimerAttr()
{
}

void TimerAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(elapse);
	jnode["target"] = target.toString(Qt::ISODate);
	vtoj(day);
	vtoj(hour);
	vtoj(minute);
	vtoj(second);
	vtoj(unit);
	vtoj(unitColor);
	vtoj(digitColor);
	vtoj(family);
	vtoj(point);
	vtoj(bold);
	vtoj(italic);
	vtoj(underline);
	vtoj(extraText);
	vtoj(text);
	vtoj(extraLine);
	vtoj(textColor);
}

QString TimerAttr::exportCoreJsonArea(Json& jArea)
{
	QRect rectContent = this->contentRect->rect();
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

	QRect refStrRect = framePainter.boundingRect(rectContent, flags, refStr);

	int varWidth = refStrRect.width() * 2;
	int timerWidth = 0;

	QString dayUnitStr{tr("Day")};
	QRect dayUnitRect;
	if (day) {
		timerWidth += varWidth;
		if (unit) {
			dayUnitRect = framePainter.boundingRect(rectContent, flags, dayUnitStr);
			timerWidth += dayUnitRect.width();
		}
	}

	QString hourUnitStr{tr("Hour")};
	QRect hourUnitRect;
	if (hour) {
		timerWidth += varWidth;
		if (unit) {
			hourUnitRect = framePainter.boundingRect(rectContent, flags, hourUnitStr);
			timerWidth += hourUnitRect.width();
		}
	}

	QString minuteUnitStr{tr("Minute")};
	QRect minuteUnitRect;
	if (minute) {
		timerWidth += varWidth;
		if (unit) {
			minuteUnitRect = framePainter.boundingRect(rectContent, flags, minuteUnitStr);
			timerWidth += minuteUnitRect.width();
		}
	}

	QString secondUnitStr{tr("Second")};
	QRect secondUnitRect;
	if (second) {
		timerWidth += varWidth;
		if (unit) {
			secondUnitRect = framePainter.boundingRect(rectContent, flags, secondUnitStr);
			timerWidth += secondUnitRect.width();
		}
	}

	int timerX = 0;
	int timerY = 0;

	if (extraText && !text.isEmpty()) {
		QRect textRect = framePainter.boundingRect(rectContent, flags, text);

		int gapX = framePix.width() - textRect.width();
		int gapY = framePix.height() - textRect.height();
		if (extraLine) {
			gapY -= refStrRect.height();
		} else {
			gapX -= timerWidth;
		}
		textRect.moveTo(gapX / 2, gapY / 2);

		framePainter.setPen(QColor{textColor});
		framePainter.drawText(textRect, flags, text);

		if (extraLine) {
			timerX = (framePix.width() - timerWidth) / 2;
			timerY = textRect.y() + textRect.height();
		} else {
			timerX = textRect.x() + textRect.width();
			timerY = textRect.y();
		}
	} else {
		timerX = (framePix.width() - timerWidth) / 2;
		timerY = (framePix.height() - refStrRect.height()) / 2;
	}

	QString framePixFile = UtilQt::uuidFile(setting->tempDir(), ".bmp");
	framePix.save(framePixFile);

	QPixmap timerPix{timerWidth - varWidth, refStrRect.height()};
	timerPix.fill(Qt::transparent);
	QPainter timerPainter(&timerPix);
	timerPainter.setFont(font);
	timerPainter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
	timerPainter.setPen(QColor{unitColor});

	int x = -varWidth;

	if (day) {
		x += varWidth;
		dayUnitRect.moveTo(x, 0);
		if (unit) {
			timerPainter.drawText(dayUnitRect, flags, dayUnitStr);
			x += dayUnitRect.width();
		}
	}

	if (hour) {
		x += varWidth;
		hourUnitRect.moveTo(x, 0);
		if (unit) {
			timerPainter.drawText(hourUnitRect, flags, hourUnitStr);
			x += hourUnitRect.width();
		}
	}

	if (minute) {
		x += varWidth;
		minuteUnitRect.moveTo(x, 0);
		if (unit) {
			timerPainter.drawText(minuteUnitRect, flags, minuteUnitStr);
			x += minuteUnitRect.width();
		}
	}

	if (second) {
		x += varWidth;
		secondUnitRect.moveTo(x, 0);
		if (unit) {
			timerPainter.drawText(secondUnitRect, flags, secondUnitStr);
			x += secondUnitRect.width();
		}
	}

	QString timerPixFile = UtilQt::uuidFile(setting->tempDir(), ".bmp");
	timerPix.save(timerPixFile);

	//digit
	Json jDigit;
	for (int i = 0; i < 10; ++i) {
		QPixmap digitPix{refStrRect.size()};
		digitPix.fill(Qt::transparent);
		QPainter digitPainter(&digitPix);
		digitPainter.setFont(font);
		digitPainter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
		digitPainter.setPen(QColor{digitColor});
		digitPainter.drawText(refStrRect, flags, QString::number(i));
		QString numberPixFile = UtilQt::uuidFile(setting->tempDir(), ".bmp");
		digitPix.save(numberPixFile);
		jDigit.push_back(numberPixFile);
	}

	int week = target.date().dayOfWeek();
	if (week == 7 ) week = 0;
	jArea = {
		{"timingMode", elapse ? "up" : "down"},
		{"enTurn", false},
		{"showDay", day},
		{"showHour", hour},
		{"showMin", minute},
		{"showSec", second},
		{"aimTime", {
			 {"date", target.date().toString(Qt::ISODate)},
			 {"week", week},
			 {"time", target.time().toString(Qt::ISODate)}
			}
		},
		{"backgroundContent", framePixFile},
		{"digitX", timerX},
		{"digitY", timerY},
		{"secondContent", timerPixFile},
		{"secondPictureWidth", timerPix.width()},
		{"secondPictureHeight", timerPix.height()},
		{"dayWidth", dayUnitRect.width()},
		{"hourWidth", hourUnitRect.width()},
		{"minWidth", minuteUnitRect.width()},
		{"secWidth", secondUnitRect.width()},
		{"digitFontWidth", refStrRect.width()},
		{"digitfontHeight", refStrRect.height()},
		{"digitPictureWidth", refStrRect.width()},
		{"digitPictureHeight", refStrRect.height()},
		{"digitContentArray", jDigit},
	};

	return QString();
}

