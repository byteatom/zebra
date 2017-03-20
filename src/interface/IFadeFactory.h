#ifndef IFADEFACTORY_H
#define IFADEFACTORY_H

#include "IFade.h"

class IFadeFactory
{
public:
	virtual IFade* create(Json* jnode, int defaultStyle, bool hopsDisabled) = 0;
protected:
	virtual ~IFadeFactory() = default;
};

#endif // IFADEFACTORY_H
