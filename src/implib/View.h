#ifndef VIEW_H
#define VIEW_H

#include <QWidget>

#include "NodeAttr.h"

class View : public QWidget
{
public:
	virtual void activate()
	{
		active = true;
		show();
		update();
	}

	virtual void deactive()
	{
		active = false;
		update();
	}

	qreal zoomFactor = 1;
protected:
	explicit View(NodeAttr* attr):
		attr{attr}
	{
	}

	virtual ~View() = default;

	bool active = false;
private:
	NodeAttr* attr = nullptr;
};

#endif // VIEW_H
