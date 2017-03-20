#ifndef VARIABLEVIEW_H
#define VARIABLEVIEW_H

#include <QPixmap>

#include "RegionView.h"
#include "VariableAttr.h"

class VariableView : public RegionView
{
public:
	explicit VariableView(VariableAttr* attr);
	virtual ~VariableView() = default;

private:
	virtual void play() override;
	virtual void draw()override;
	virtual void stop()override;

	VariableAttr* attr = nullptr;
};

#endif // VARIABLEVIEW_H
