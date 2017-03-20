#ifndef VARIABLECFGER_H
#define VARIABLECFGER_H

#include "RegionCfger.h"
#include "VariableAttr.h"
#include "VariableAttrCfger.h"

class VariableCfger : public RegionCfger
{
	Q_OBJECT
public:
	explicit VariableCfger(VariableAttr *attr);
	virtual ~VariableCfger();

private:
	VariableAttr* attr = nullptr;
	VariableAttrCfger* attrCfger = nullptr;
};

#endif // VARIABLECFGER_H
