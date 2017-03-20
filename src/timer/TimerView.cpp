#include "TimerView.h"

TimerView::TimerView(TimerAttr* attr):
	RegionView{attr},
	attr{attr}
{
}

void TimerView::play()
{
	RegionView::play();
}

void TimerView::draw()
{
	QRect rectContent = attr->contentRect->rect();

	QFont font{attr->family,
				attr->point,
				attr->bold ? QFont::Bold : QFont::Normal,
				attr->italic};
	font.setUnderline(attr->underline);
	font.setStyleStrategy(QFont::NoAntialias);
	QFontMetrics metrics{font};

	//date time
	static const int secMinute = 60;
	static const int secHour = secMinute * 60;
	static const int secDay = secHour * 24;

	QDateTime now{QDateTime::currentDateTime()};
	int second = 0;

	if (attr->elapse && now > attr->target) {
		second = attr->target.secsTo(now);
	} else if (!attr->elapse && now < attr->target) {
		second = now.secsTo(attr->target);
	}

	QString dayDigitStr;
	QRect dayDigitRect;
	QString dayUnitStr;
	QRect dayUnitRect;
	if (attr->day) {
		int day = second / secDay;
		dayDigitStr = QString("%1").arg(day, 2, 10, QLatin1Char{'0'});
		if (attr->unit) {
			dayUnitStr = tr("Day");
			dayUnitRect = metrics.boundingRect(dayUnitStr);
		} else
			dayDigitStr.append(" ");
		dayDigitRect = metrics.boundingRect(dayDigitStr);
		second = second % secDay;
	}

	QString hourDigitStr;
	QRect hourDigitRect;
	QString hourUnitStr;
	QRect hourUnitRect;
	if (attr->hour) {
		int hour = second / secHour;
		hourDigitStr = QString("%1").arg(hour, 2, 10, QLatin1Char{'0'});
		if (attr->unit) {
			hourUnitStr = tr("Hour");
			hourUnitRect = metrics.boundingRect(hourUnitStr);
		} else
			hourDigitStr.append(" ");
		hourDigitRect = metrics.boundingRect(hourDigitStr);
		second = second % secHour;
	}

	QString minuteDigitStr;
	QRect minuteDigitRect;
	QString minuteUnitStr;
	QRect minuteUnitRect;
	if (attr->minute) {
		int minute = second / secMinute;
		minuteDigitStr = QString("%1").arg(minute, 2, 10, QLatin1Char{'0'});
		if (attr->unit) {
			minuteUnitStr = tr("Minute");
			minuteUnitRect = metrics.boundingRect(minuteUnitStr);
		} else
			minuteDigitStr.append(" ");
		minuteDigitRect = metrics.boundingRect(minuteDigitStr);
		second = second % secMinute;
	}

	QString secondDigitStr;
	QRect secondDigitRect;
	QString secondUnitStr;
	QRect secondUnitRect;
	if (attr->second) {
		secondDigitStr = QString("%1").arg(second, 2, 10, QLatin1Char{'0'});
		if (attr->unit) {
			secondUnitStr = tr("Second");
			secondUnitRect = metrics.boundingRect(secondUnitStr);
		}
		secondDigitRect = metrics.boundingRect(secondDigitStr);
	}

	QPainter painter(&sight);
	painter.setFont(font);
	painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);

	int timerWidth = dayDigitRect.width() + dayUnitRect.width()
					 + hourDigitRect.width() + hourUnitRect.width()
					 + minuteDigitRect.width() + minuteUnitRect.width()
					 + secondDigitRect.width() + secondUnitRect.width();
	int timerHeight = std::max<int>(std::max<int>(std::max<int>(
												  dayDigitRect.height(),
												  hourDigitRect.height()),
												  minuteDigitRect.height()),
												  secondDigitRect.height());
	int timerFontY = std::min<int>(std::min<int>(std::min<int>(
													 dayDigitRect.y(),
													 hourDigitRect.y()),
													 minuteDigitRect.y()),
													 secondDigitRect.y());
	int timerX = (rectContent.width() - timerWidth) / 2 + rectContent.x();
	int timerY = (rectContent.height() - timerHeight) / 2 + rectContent.y() - timerFontY;

	if (attr->extraText && !attr->text.isEmpty()) {
		QRect textRect = metrics.boundingRect(attr->text);
		int xSpace = rectContent.width() - textRect.width();
		int ySpace = rectContent.height() - textRect.height();
		if (attr->extraLine) {
			ySpace -= timerHeight;
		} else {
			xSpace -= timerWidth;
		}
		int textX = xSpace / 2 + rectContent.x();
		int textY = ySpace / 2 - textRect.y() + rectContent.y();
		painter.setPen(QColor{attr->textColor});
		painter.drawText(textX, textY, attr->text);

		if (attr->extraLine) {
			timerY = textY + textRect.height();
		} else {
			timerX = textX + textRect.width();
		}
	}

	if (attr->day) {
		painter.setPen(QColor{attr->digitColor});
		painter.drawText(timerX, timerY, dayDigitStr);
		timerX += dayDigitRect.width();
		if (attr->unit) {
			painter.setPen(QColor{attr->unitColor});
			painter.drawText(timerX, timerY, dayUnitStr);
			timerX += dayUnitRect.width();
		}
	}
	if (attr->hour) {
		painter.setPen(QColor{attr->digitColor});
		painter.drawText(timerX, timerY,hourDigitStr);
		timerX += hourDigitRect.width();
		if (attr->unit) {
			painter.setPen(QColor{attr->unitColor});
			painter.drawText(timerX, timerY, hourUnitStr);
			timerX += hourUnitRect.width();
		}
	}
	if (attr->minute) {
		painter.setPen(QColor{attr->digitColor});
		painter.drawText(timerX, timerY,minuteDigitStr);
		timerX += minuteDigitRect.width();
		if (attr->unit) {
			painter.setPen(QColor{attr->unitColor});
			painter.drawText(timerX, timerY, minuteUnitStr);
			timerX += minuteUnitRect.width();
		}
	}
	if (attr->second) {
		painter.setPen(QColor{attr->digitColor});
		painter.drawText(timerX, timerY,secondDigitStr);
		timerX += secondDigitRect.width();
		if (attr->unit) {
			painter.setPen(QColor{attr->unitColor});
			painter.drawText(timerX, timerY, secondUnitStr);
			timerX += secondUnitRect.width();
		}
	}
}

void TimerView::stop()
{
	RegionView::stop();
}
