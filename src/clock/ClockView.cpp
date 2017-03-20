#include "ClockView.h"

#include <QtGlobal>
#include <QtMath>
#include <QTime>
#include <QStaticText>
#include <QPainter>

ClockView::ClockView(ClockAttr* attr):
	RegionView{attr},
	attr{attr}
{
	memset(&inputBK, 0, sizeof inputBK);
}

void ClockView::play()
{
	RegionView::play();
}

void ClockView::draw()
{
	QRect rectContent = attr->contentRect->rect();

	attr->toInputAttr(KG_CREATE_MEMORY);
	if (memcmp(&inputBK, &attr->input, sizeof(inputBK))) {
		freeWatchArea(&attr->output);
		watchAreaWork(&attr->input, &attr->output);
		memcpy(&inputBK, &attr->input, sizeof(inputBK));
	}

	QPainter painter(&sight);

	QImage image(attr->output.bitmap.data,
				 attr->output.bitmap.width,
				 attr->output.bitmap.height,
				 QImage::Format_ARGB32);
	painter.drawPixmap(rectContent.x(), rectContent.y(), QPixmap::fromImage(image),
					   0, 0, rectContent.width(), rectContent.height());

	int i = QDateTime::currentDateTime().time().second();
	QImage imageSec(attr->output.secHand[i].bitmap.data,
					 attr->output.secHand[i].bitmap.width,
					 attr->output.secHand[i].bitmap.height,
					 QImage::Format_ARGB32);
	painter.drawPixmap(rectContent.x() + attr->output.secHand[i].coord.x,
					   rectContent.y() + attr->output.secHand[i].coord.y,
					   QPixmap::fromImage(imageSec), 0, 0,
					   attr->output.secHand[i].coord.width,
					   attr->output.secHand[i].coord.height);

	i = QDateTime::currentDateTime().time().minute();
	QImage imageMin(attr->output.minHand[i].bitmap.data,
					 attr->output.minHand[i].bitmap.width,
					 attr->output.minHand[i].bitmap.height,
					 QImage::Format_ARGB32);
	painter.drawPixmap(rectContent.x() + attr->output.minHand[i].coord.x,
					   rectContent.y() + attr->output.minHand[i].coord.y,
					   QPixmap::fromImage(imageMin), 0, 0,
					   attr->output.minHand[i].coord.width,
					   attr->output.minHand[i].coord.height);

	i = (QDateTime::currentDateTime().time().hour()%12)*5 +
			QDateTime::currentDateTime().time().minute()/12;
	QImage imageHour(attr->output.hourHand[i].bitmap.data,
					 attr->output.hourHand[i].bitmap.width,
					 attr->output.hourHand[i].bitmap.height,
					 QImage::Format_ARGB32);
	painter.drawPixmap(rectContent.x() + attr->output.hourHand[i].coord.x,
					   rectContent.y() + attr->output.hourHand[i].coord.y,
					   QPixmap::fromImage(imageHour), 0, 0,
					   attr->output.hourHand[i].coord.width,
					   attr->output.hourHand[i].coord.height);

/*
	QPainter painter(&sight);

	painter.drawPixmap(content.x(), content.y(), drawWatch(),
					   0, 0, content.width(), content.height());

*/
}

void ClockView::stop()
{
	RegionView::stop();
}
