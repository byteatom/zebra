#ifndef SCREENVIEW_H
#define SCREENVIEW_H

#include <QStackedLayout>

#include "View.h"
#include "ScreenAttr.h"
#include "MouseGeometry.h"

class ScreenView : public View
{
	Q_OBJECT
public:
	explicit ScreenView(ScreenAttr *attr);
	virtual ~ScreenView() = default;

	void attach(QWidget* child);
	void activate(QWidget* child);
	void zoom(qreal factor);
private:
	virtual void paintEvent(QPaintEvent *event) override;

	ScreenAttr *attr = nullptr;
	QStackedLayout layout;
	MouseGeometry mouseGeometry;
};

#endif // SCREENVIEW_H
