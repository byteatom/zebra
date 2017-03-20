#ifndef FADEMANAGER_H
#define FADEMANAGER_H

#include "IPlugin.h"
#include "IFadeFactory.h"

class FadeManager : public IPlugin, public IFadeFactory
{
	virtual ~FadeManager() = default;

	virtual IFade* create(Json* jnode, int defaultStyle, bool hopsDisabled) override;
};

#endif // FADEMANAGER_H
