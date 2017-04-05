#ifndef CFGER_H
#define CFGER_H

#include <algorithm>

#include <QFrame>

#include "NodeAttr.h"

class Cfger : public QFrame
{
	Q_OBJECT
protected:
	explicit Cfger(NodeAttr* attr):
		attr{attr}
	{
	}

	virtual ~Cfger() = default;

	void append(QWidget* cfger)
	{
		if (!cfger) return;

		cfger->setParent(this);
		cfger->move(width(), UI_SPACE);
		cfger->show();
		resize(width() + cfger->width() + UI_SPACE, std::max<int>(height(), cfger->height() + UI_SPACE));
	}

private:
	NodeAttr* attr = nullptr;
};

#endif // CFGER_H
