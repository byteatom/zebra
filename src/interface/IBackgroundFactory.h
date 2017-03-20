#ifndef IBACKGROUNDFACTORY_H
#define IBACKGROUNDFACTORY_H

#include "IBackground.h"

class IBackgroundFactory
{
public:
	virtual IBackground* create(Json* jnode) = 0;
protected:
	virtual ~IBackgroundFactory() = default;
};

#endif // IBACKGROUNDFACTORY_H
