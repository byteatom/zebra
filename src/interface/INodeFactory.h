#ifndef INODEFACTORY_H
#define INODEFACTORY_H

#include "INode.h"
#include "Json.h"

class INodeFactory
{
public:
	virtual INode::Type type() = 0;
	virtual void create() = 0;
	virtual INode* create(INodeBox* box, Json& jnode) = 0;
protected:
	virtual ~INodeFactory() = default;
};

#endif // INODEFACTORY_H
