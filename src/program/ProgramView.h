#ifndef PROGRAMVIEW_H
#define PROGRAMVIEW_H

#include "View.h"
#include "ProgramAttr.h"
#include "INode.h"

class ProgramView : public View
{
	Q_OBJECT
public:
	explicit ProgramView(ProgramAttr *attr);
	virtual ~ProgramView() = default;

	void attach(QWidget* child);
	void activate(QWidget* child);

	virtual void play();
	virtual void draw();
	virtual void stop();
	void update();
	void refresh(const QSize& size);
	void zoom(qreal factor);

	INodeData* data = nullptr;
private:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent(QMouseEvent *event) override;

	ProgramAttr *attr = nullptr;
	QPixmap sight;
};

#endif // PROGRAMVIEW_H
