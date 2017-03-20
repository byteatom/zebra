#include "CalendarView.h"

CalendarView::CalendarView(CalendarAttr* attr):
	RegionView{attr},
	attr{attr}
{
}

void CalendarView::play()
{
	RegionView::play();
}

void CalendarView::draw()
{
	QRect rectContent = attr->contentRect->rect();

	QFont font{attr->family,
				attr->point,
				attr->bold ? QFont::Bold : QFont::Normal,
				attr->italic};
	font.setUnderline(attr->underline);
	font.setStyleStrategy(QFont::NoAntialias);
	QFontMetrics metrics{font};

	QDateTime now{QDateTime::currentDateTime()};
	if (attr->offset == 0) {
		now = now.addSecs(attr->offsetHour*3600 + attr->offsetMinute*60);
	} else {
		now = now.addSecs(-attr->offsetHour*3600 - attr->offsetMinute*60);
	}
	QDate date = now.date();
	QTime time = now.time();

	int spaceCount = 0;

	QString dateStr;
	QRect dateRect;
	if (attr->date) {
		dateStr = date.toString(CalendarAttr::dateStyles[attr->dateStyle]->format);
		dateRect = metrics.boundingRect(dateStr);
		spaceCount++;
	}
	QString weekStr;
	QRect weekRect;
	if (attr->week) {
		weekStr = CalendarAttr::weekStyles[attr->weekStyle][date.dayOfWeek() - 1];
		weekRect = metrics.boundingRect(weekStr);
		spaceCount++;
	}
	QString timeStr;
	QRect timeRect;
	if (attr->time) {
		timeStr = time.toString(CalendarAttr::timeStyles[attr->timeStyle]->format);
		timeRect = metrics.boundingRect(timeStr);
		spaceCount++;
	}
	QRect textRect;
	if (attr->extraText && !attr->text.isEmpty()) {
		textRect = metrics.boundingRect(attr->text);
		spaceCount++;
	}
	if (spaceCount > 0) spaceCount--;

	QPainter painter(&sight);
	painter.setFont(font);
	painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);

	if (attr->multiLine) {
		int y = (rectContent.height() - textRect.height() - dateRect.height()
				- weekRect.height() - timeRect.height() - spaceCount*attr->space)
				/ 2 + rectContent.y();

		if (attr->extraText && !attr->text.isEmpty()) {
			painter.setPen(QColor{attr->textColor});
			painter.drawText((rectContent.width() - textRect.width()) / 2 + rectContent.x(),
							 y - textRect.y(), attr->text);
			y += (textRect.height() + attr->space);
		}

		if (attr->dateFirst) {
			if (attr->date) {
				painter.setPen(QColor{attr->dateColor});
				painter.drawText((rectContent.width() - dateRect.width()) / 2 + rectContent.x(),
								 y - dateRect.y(), dateStr);
				y += (dateRect.height() + attr->space);
			}

			if (attr->week) {
				painter.setPen(QColor{attr->weekColor});
				painter.drawText((rectContent.width() - weekRect.width()) / 2 + rectContent.x(),
								 y - weekRect.y(), weekStr);
				y += (weekRect.height() + attr->space);
			}
		} else {
			if (attr->week) {
				painter.setPen(QColor{attr->weekColor});
				painter.drawText((rectContent.width() - weekRect.width()) / 2 + rectContent.x(),
								 y - weekRect.y(), weekStr);
				y += (weekRect.height() + attr->space);
			}

			if (attr->date) {
				painter.setPen(QColor{attr->dateColor});
				painter.drawText((rectContent.width() - dateRect.width()) / 2 + rectContent.x(),
								 y - dateRect.y(), dateStr);
				y += (dateRect.height() + attr->space);
			}
		}

		if (attr->time) {
			painter.setPen(QColor{attr->timeColor});
			painter.drawText((rectContent.width() - timeRect.width()) / 2 + rectContent.x(),
							 y - timeRect.y(), timeStr);
		}
	} else {
		int x = (rectContent.width() - textRect.width() - dateRect.width()
				- weekRect.width() - timeRect.width() - spaceCount*attr->space)
				/ 2 + rectContent.x();

		if (attr->extraText && !attr->text.isEmpty()) {
			painter.setPen(QColor{attr->textColor});
			painter.drawText(x,	 (rectContent.height() - textRect.height()) / 2 + rectContent.y() - textRect.y(),
							 attr->text);
			x += (textRect.width() + attr->space);
		}

		if (attr->dateFirst) {
			if (attr->date) {
				painter.setPen(QColor{attr->dateColor});
				painter.drawText(x,	 (rectContent.height() - dateRect.height()) / 2 + rectContent.y() - dateRect.y(),
								 dateStr);
				x += (dateRect.width() + attr->space);
			}

			if (attr->week) {
				painter.setPen(QColor{attr->weekColor});
				painter.drawText(x,	 (rectContent.height() - weekRect.height()) / 2 + rectContent.y() - weekRect.y(),
								 weekStr);
				x += (weekRect.width() + attr->space);
			}
		} else {
			if (attr->week) {
				painter.setPen(QColor{attr->weekColor});
				painter.drawText(x,	 (rectContent.height() - weekRect.height()) / 2 + rectContent.y() - weekRect.y(),
								 weekStr);
				x += (weekRect.width() + attr->space);
			}

			if (attr->date) {
				painter.setPen(QColor{attr->dateColor});
				painter.drawText(x,	 (rectContent.height() - dateRect.height()) / 2 + rectContent.y() - dateRect.y(),
								 dateStr);
				x += (dateRect.width() + attr->space);
			}
		}

		if (attr->time) {
			painter.setPen(QColor{attr->timeColor});
			painter.drawText(x,	 (rectContent.height() - timeRect.height()) / 2 + rectContent.y() - timeRect.y(),
							 timeStr);
		}
	}
}

void CalendarView::stop()
{
	RegionView::stop();
}
