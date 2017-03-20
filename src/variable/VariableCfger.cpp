#include "VariableCfger.h"

VariableCfger::VariableCfger(VariableAttr *attr) :
	RegionCfger(attr),
	attr{attr},
	attrCfger{new VariableAttrCfger{this, attr}}
{
	append(attrCfger);
	appendBorder();
	appendGlow();
}

VariableCfger::~VariableCfger()
{
}



