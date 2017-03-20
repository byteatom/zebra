#ifndef VARIABLE_H
#define VARIABLE_H

#include "VariableView.h"
#include "VariableCfger.h"
#include "VariableAttr.h"
#include "Region.h"

struct VariableData
{
	explicit VariableData(INodeBox* box, Json* jnode);
	~VariableData();

	VariableAttr* attr;
	VariableView* view;
	VariableCfger* cfger;
};

class Variable : private VariableData, public Region
{
public:
	explicit Variable(INodeBox* box, Json* jnode);

private:
	virtual ~Variable() = default;

	virtual void exportProjJson(Json& jnode) override;

	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	using VariableData::attr;
	using VariableData::view;
	using VariableData::cfger;
};

#endif // VARIABLE_H
