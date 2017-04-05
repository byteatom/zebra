#ifndef REGIONVIEW_H
#define REGIONVIEW_H

#include <QPainter>
#include <QPixmap>

#include "View.h"
#include "MouseGeometry.h"
#include "INode.h"
#include "RegionAttr.h"
#include "IBorder.h"

class RegionView : public View
{
public:
	INodeData* data = nullptr;

	virtual void play()
	{
		playing = true;
		if (attr->border) attr->border->play();
	}

	virtual void draw() = 0;

	void drawAll()
	{
		if (sight.size() != attr->geometry->size) {
			sight = QPixmap{attr->geometry->size};
		}
		sight.fill(Qt::transparent);
		if (attr->border) attr->border->draw();
		draw();
	}

	virtual void stop()
	{
		playing = false;
		if (attr->border) attr->border->stop();
	}

	void update()
	{
		drawAll();
		View::update();
	}

	void setGeometry()
	{
		View::setGeometry(QRect(attr->geometry->point.get() * zoomFactor,
						  attr->geometry->size.get() * zoomFactor));
	}
protected:
	explicit RegionView(RegionAttr* attr_):
		View{attr_},
		attr{attr_},
		mouseGeometry{this},
		playing{false}
	{
		setStyleSheet("background-color: transparent");
		zoomFactor = attr->screen()->zoomFactor();
		setGeometry();
		attr->geometry.connect([this](){setGeometry();});

		attr->contentRect.connect([this](){attr->changed();}, boost::signals2::at_back);
		attr->changed.connect([this](){update();});

		mouseGeometry.canOverstepParent = false;
		mouseGeometry.changing.connect([this](QRectF& rectf){
			QRect rect = QRectF(rectf.topLeft() / zoomFactor,
								rectf.size() / zoomFactor)
						 .toRect();
			QRect boxRect = attr->box->contentRect()->rect();
			if (rect.width() < RegionAttr::MIN_SIZE
				|| rect.height() < RegionAttr::MIN_SIZE
				|| rect.x() < boxRect.x()
				|| rect.y() < boxRect.y()
				|| rect.right() > boxRect.right()
				|| rect.bottom() > boxRect.bottom())
				return;
			attr->geometry->setRect(rect);
		});

		if (attr->border) attr->border->setSurface(&sight);
	}

	virtual ~RegionView() = default;

	virtual void mousePressEvent(QMouseEvent *event) override
	{
		if (event->spontaneous() && data)
		{
			data->activate();
		}
	}


	void paintEvent(QPaintEvent *event) override
	{
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing, false);
		QRect qrect = rect();
		painter.drawPixmap(qrect, sight);
		if (!playing) {
#define EDITING_BORDER_SIZE 1
			painter.setPen(QPen{QBrush{active ? 0x0078D7 : 0x7A7A7A}, EDITING_BORDER_SIZE});
			qrect.adjust(0, 0, -EDITING_BORDER_SIZE, -EDITING_BORDER_SIZE);
			painter.drawRect(qrect);
		}
	}

private:
	RegionAttr* attr = nullptr;
	MouseGeometry mouseGeometry;
protected:
	bool playing;
	QPixmap sight;
};

#endif // REGIONVIEW_H
