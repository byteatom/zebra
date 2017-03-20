#ifndef IGLOWFACTORY_H
#define IGLOWFACTORY_H

#include "IGlow.h"
#include "AttrRect.h"

class IGlowFactory
{
public:
	virtual IGlow* create(Json* jnode, Attr<KRect>* geometry, Attr<KRect>* contentRect) = 0;
protected:
	virtual ~IGlowFactory() = default;
};

#endif // IGLOWFACTORY_H
