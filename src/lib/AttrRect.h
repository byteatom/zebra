#ifndef ATTRRECT_H
#define ATTRRECT_H

#include <QRect>
#include <QPoint>
#include <QSize>

#include "Attr.h"

struct KRect {
	Attr<QPoint> point;
	Attr<QSize> size;

	KRect(Attr<KRect>* attr_) :
		point{0, 0}, size{0, 0}, attr{attr_}
	{
		auto slot = [this](){
			if (notify) attr->changed();
		};
		point.connect(slot);
		size.connect(slot);
	}

	QRect rect() const
	{
		return QRect{point.get(), size.get()};
	}

	void setRect(const QPoint& p, const QSize& s)
	{
		if (p != point.ref() && s != size.ref())
			notify = false;
		point = p;
		notify = true;
		size = s;
	}

	void setRect(const int x, const int y, const int width, const int height)
	{
		QPoint p{x, y};
		QSize s{width, height};
		setRect(p, s);
	}

	void setRect(const QRect& r)
	{
		QPoint& p = r.topLeft();
		QSize& s = r.size();
		setRect(p, s);
	}

	void operator=(const QRect &r)
	{
		setRect(r);
	}

	void operator=(const KRect &r)
	{
		setRect(r.point, r.size);
	}

	bool operator!=(const KRect &r)
	{
		return (point.get() != r.point.get() || size.get() != r.size.get());
	}

	int right() const
	{
		return point->x()  + size->width() - 1;
	}

	int rightReal() const
	{
		return point->x()  + size->width();
	}

	int bottom() const
	{
		return point->y() + size->height() - 1;
	}

	int bottomReal() const
	{
		return point->y() + size->height();
	}

private:
	Attr<KRect>* attr;
	bool notify = true;
};

#endif // ATTRRECT_H
