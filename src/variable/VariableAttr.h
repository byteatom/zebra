#ifndef VARIABLEATTR_H
#define VARIABLEATTR_H

#include <boost/signals2.hpp>

#include "RegionAttr.h"

class VariableAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(VariableAttr)
public:
	explicit VariableAttr(INodeBox* box, Json* jnode);
	virtual ~VariableAttr();

	int id = 0;
	int x = 0;
	int y = 0;
	QStr text;
	QStr family;
	int point = 10;
	QRgb color = qRgba(255, 0, 0, 255);

	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // VARIABLEATTR_H
