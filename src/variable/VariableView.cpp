#include "VariableView.h"

VariableView::VariableView(VariableAttr* attr):
	RegionView{attr},
	attr{attr}
{
}

void VariableView::play()
{
	RegionView::play();
}

void VariableView::draw()
{
	QRect rectContent = attr->contentRect->rect();
	QFont font{attr->family,
				attr->point,
				QFont::Normal,
				false};
	font.setStyleStrategy(QFont::NoAntialias);
	QFontMetrics metrics{font};
	QPainter painter(&sight);
	painter.setFont(font);
	painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
	painter.setPen(QColor{attr->color});
	QRect textRect = metrics.boundingRect(attr->text);
	painter.drawText(attr->x + rectContent.x(), attr->y + rectContent.y() - textRect.y(), attr->text);
}

void VariableView::stop()
{
	RegionView::stop();
}
