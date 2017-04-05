#include "ScreenView.h"

#include <QPainter>

ScreenView::ScreenView(ScreenAttr *attr):
	View{attr},
	attr{attr},
	mouseGeometry{this}
{
	setLayout(&layout);
	resize(attr->para.size);

	attr->para.size.connect([this]() {
		zoom(zoomFactor);
	});
	mouseGeometry.canResize = true;
	mouseGeometry.canMove = false;
	mouseGeometry.changing.connect([this](QRectF& rectf){
		this->attr->para.size = (rectf.size() / zoomFactor).toSize();
	});
}

void ScreenView::attach(QWidget* child)
{
	layout.addWidget(child);
}

void ScreenView::activate(QWidget* child)
{
	layout.setCurrentWidget(child);
}

void ScreenView::zoom(qreal factor)
{
	zoomFactor = factor;
	resize(attr->para.size * factor);
}

void ScreenView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect qrect = rect();
	int width = qrect.width();
	int height = qrect.height();
	painter.fillRect(qrect, Qt::black);
	if (zoomFactor > 2) {
		painter.setPen(QColor(0x222222));
		for (int x = 0; x < width; x += zoomFactor) {
			painter.drawLine(x, 0, x, height);
		}
		for (int y = 0; y < height; y += zoomFactor) {
			painter.drawLine(0, y, width, y);
		}
	}
}
