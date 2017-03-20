#include "Variable.h"

VariableData::VariableData(INodeBox* box, Json* jnode)
{
	attr = new VariableAttr{box, jnode},
	view = new VariableView{attr},
	cfger = new VariableCfger{attr};
}

VariableData::~VariableData()
{
	delete cfger;
	delete view;
	delete attr;
}

Variable::Variable(INodeBox *box, Json* jnode):
	VariableData{box, jnode},
	Region{attr, view, cfger}
{
}

void Variable::exportProjJson(Json& jnode)
{
	attr->exportProjJson(jnode);
}

void Variable::play()
{
	Region::play();
}

void Variable::draw()
{
	Region::draw();
}

void Variable::stop()
{
	Region::stop();
}
