#include "ProgramView.h"

#include <QMouseEvent>
#include <QPainter>

ProgramView::ProgramView(ProgramAttr *attr):
	View{attr},
	attr{attr}
{
	setMouseTracking(true);

	if (attr->background) {
		attr->background->setSurface(&sight);
		attr->background->cfgChanged([this](){
			update();
		});
	}

	if (attr->border) {
		attr->border->setSurface(&sight);
		attr->border->cfgChanged([this](){
			update();
		});
	}
}

void ProgramView::attach(QWidget* child)
{
	child->setParent(this);
}

void ProgramView::activate(QWidget* child)
{
	child->raise();
}

void ProgramView::play()
{
	if (attr->background) attr->background->play();
	if (attr->border) attr->border->play();
}

void ProgramView::draw()
{
	sight.fill(Qt::transparent);
	if (attr->background) attr->background->draw();
	if (attr->border) attr->border->draw();
}

void ProgramView::stop()
{
	if (attr->background) attr->background->stop();
	if (attr->border) attr->border->stop();
}

void ProgramView::update()
{
	draw();
	View::update();
}

void ProgramView::refresh(const QSize& size)
{
	if (sight.size() == size) return;
	sight = QPixmap{size};
	update();
}

void ProgramView::zoom(qreal factor)
{
	zoomFactor = factor;
	QSizeF realSizeF{width() / zoomFactor, height() / zoomFactor};
	refresh(realSizeF.toSize());
}

void ProgramView::resizeEvent(QResizeEvent *event)
{
	refresh(event->size());
}

void ProgramView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
	QRect qrect = rect();
	painter.drawPixmap(qrect, sight);
}

void ProgramView::mousePressEvent(QMouseEvent *event)
{
	if (event->spontaneous() && data)
		data->activate();
	View::mousePressEvent(event);
}
