#include "VariableAttr.h"

VariableAttr::VariableAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::VARIABLE, jnode},
	id{1/*box->applyNodeId()*/}
{
	if (jnode) {
		jtov(id);
		jtov(x);
		jtov(y);
		jtov(text);
		jtov(family);
		jtov(point);
		jtov(color);
	}
}

VariableAttr::~VariableAttr()
{
}

void VariableAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(id);
	vtoj(x);
	vtoj(y);
	vtoj(text);
	vtoj(family);
	vtoj(point);
	vtoj(color);
}

QString VariableAttr::exportCoreJsonArea(Json& jArea)
{
	return QString();
}

