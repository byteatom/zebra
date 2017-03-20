#ifndef ICARRIERFACTORY_H
#define ICARRIERFACTORY_H

#include "Json.h"

#include "ICarrier.h"

class ICarrierFactory
{
public:
	virtual ICarrier::TypeName typeName() = 0;
	virtual ICarrier* create(Json* jnode) = 0;
protected:
	virtual ~ICarrierFactory() = default;
};

#endif // ICARRIERFACTORY_H
