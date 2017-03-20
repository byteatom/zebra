#ifndef GLOWMANAGER_H
#define GLOWMANAGER_H

#include "IPlugin.h"
#include "IInit.h"
#include "IGlowFactory.h"

class GlowManager : public IPlugin, public IInit, public IGlowFactory
{
	virtual ~GlowManager() = default;

	virtual void init(const std::list<IPlugin*> &plugins) override;
	virtual IGlow* create(Json* jnode, Attr<KRect>* geometry, Attr<KRect>* contentRect) override;
};

#endif // GLOWMANAGER_H
