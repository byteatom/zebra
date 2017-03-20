#ifndef MOUSE_GEO_H
#define MOUSE_GEO_H

#include <boost/signals2.hpp>

#include <QWidget>
#include <QMouseEvent>

class MouseGeometry : public QObject
{
public:
	bool canResize = true;
	bool canMove = true;
	bool canOverstepParent = true;
	boost::signals2::signal<void (QRectF& rectf)> changing;

	explicit MouseGeometry(QWidget *widget):
		widget{widget}
	{
		enable(true);
	}

	virtual ~MouseGeometry()
	{
		enable(false);
	}

	void enable(bool enable)
	{
		if (enabled == enable) return;
		enabled = enable;
		if (enable) {
			widget->installEventFilter(this);
			widget->setMouseTracking(true);
		} else {
			widget->setMouseTracking(false);
			widget->removeEventFilter(this);
			widget->unsetCursor();
		}
	}

protected:
	virtual bool eventFilter(QObject* source, QEvent *event) override
	{
		if (source != widget || !widget->isEnabled()) return false;

		QMouseEvent *e = static_cast<QMouseEvent *>(event);
		switch (event->type()) {
		case QEvent::MouseButtonPress:
			mousePress(e);
			return false;
		case QEvent::MouseMove:
			mouseMove(e);
			return true;
		default:
			return false;
		}
	}

private:
	void mousePress(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton) {
			dragPos = event->screenPos();
			dragRect = widget->frameGeometry();
		}
	}

	void mouseMove(QMouseEvent *event)
	{
		if (event->buttons() == Qt::NoButton) {
			if (canResize) {
				QPointF pos = event->pos();
				QRectF rect = widget->frameGeometry();

				int bottomOffset = rect.height() - pos.y();
				int leftOffset = rect.width() - pos.x();
				onTopEdge = pos.y() < EDGE_SIZE;
				onBottomEdge = bottomOffset < EDGE_SIZE;
				if (onTopEdge && onBottomEdge) {
					onTopEdge = (pos.y() < bottomOffset);
					onBottomEdge = (pos.y() > bottomOffset);
				}
				onLeftEdge = pos.x() < EDGE_SIZE;
				onRightEdge = leftOffset < EDGE_SIZE;
				if (onLeftEdge && onRightEdge) {
					onLeftEdge = (pos.x() < leftOffset);
					onRightEdge = (pos.x() > leftOffset);
				}

				if ((onTopEdge && onLeftEdge) || (onBottomEdge && onRightEdge))
					widget->setCursor(Qt::SizeFDiagCursor);
				else if ((onTopEdge && onRightEdge) || (onBottomEdge && onLeftEdge))
					widget->setCursor(Qt::SizeBDiagCursor);
				else if (onLeftEdge || onRightEdge)
					widget->setCursor(Qt::SizeHorCursor);
				else if (onTopEdge || onBottomEdge)
					widget->setCursor(Qt::SizeVerCursor);
				else
					//widget->unsetCursor();
					widget->setCursor(Qt::ArrowCursor);
			}
		} else if (event->buttons() == Qt::LeftButton) {
			QPointF pos = event->screenPos();
			QPointF offset = pos - dragPos;
			QSize parentSize = widget->parentWidget()->frameGeometry().size();
			QRectF newRect = dragRect;
			bool onEdge = onTopEdge || onLeftEdge || onBottomEdge || onRightEdge;

			if (onEdge && canResize) {
				if (onTopEdge) {
					newRect.setTop(dragRect.top() + offset.y());
					if (!canOverstepParent && newRect.y() < 0)
						newRect.setTop(0);
				}
				if (onLeftEdge) {
					newRect.setLeft(dragRect.left() + offset.x());
					if (!canOverstepParent && newRect.x() < 0)
						newRect.setLeft(0);
				}
				if (onBottomEdge) {
					newRect.setBottom(dragRect.bottom() + offset.y());
					if (!canOverstepParent && newRect.bottom() > parentSize.height())
						newRect.setBottom(parentSize.height());
				}
				if (onRightEdge) {
					newRect.setRight(dragRect.right() + offset.x());
					if (!canOverstepParent && newRect.right() > parentSize.width())
						newRect.setRight(parentSize.width());
				}
				//widget->setGeometry(newRect.toRect());
				changing(newRect);
			} else if (!onEdge && canMove) {
				newRect.moveTo(dragRect.topLeft() + offset);
				if (!canOverstepParent) {
					if (newRect.x() < 0) newRect.moveLeft(0);
					if (newRect.y() < 0) newRect.moveTop(0);
					if (newRect.right() > parentSize.width()) newRect.moveRight(parentSize.width());
					if (newRect.bottom() > parentSize.height()) newRect.moveBottom(parentSize.height());
				}
				//widget->setGeometry(newRect.toRect());
				changing(newRect);
			}
		}
	}

	static const int EDGE_SIZE = 8;

	bool enabled = false;
	QWidget *widget;
	QPointF dragPos;
	QRectF dragRect;
	bool onTopEdge;
	bool onBottomEdge;
	bool onLeftEdge;
	bool onRightEdge;
};

#endif // MOUSE_GEO_H
