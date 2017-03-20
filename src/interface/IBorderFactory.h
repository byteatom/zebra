#ifndef IBORDERFACTORY_H
#define IBORDERFACTORY_H

#include "IBorder.h"

class IBorderFactory
{
public:
	virtual IBorder* create(Json* jnode) = 0;
protected:
	virtual ~IBorderFactory() = default;
};

#endif // IBORDERFACTORY_H
