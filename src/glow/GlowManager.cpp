#include "GlowManager.h"

DEF_PLUGIN_CREATOR(GlowManager)

#include "Glow.h"

#include "ISetting.h"

ISetting *setting = nullptr;

void GlowManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (setting) break;
	}
}

IGlow* GlowManager::create(Json* jnode, Attr<KRect>* geometry, Attr<KRect>* contentRect)
{
	return new Glow(jnode, geometry, contentRect);
}
