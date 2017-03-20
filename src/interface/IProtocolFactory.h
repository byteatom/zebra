#ifndef IPROTOCOLFACTORY_H
#define IPROTOCOLFACTORY_H

#include "Json.h"

#include "IProtocol.h"

class IProtocolFactory
{
public:
	virtual IProtocol* create(Json* jnode) = 0;
protected:
	virtual ~IProtocolFactory() = default;
};

#endif // IPROTOCOLFACTORY_H
