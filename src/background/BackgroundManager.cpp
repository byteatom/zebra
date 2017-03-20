#include "BackgroundManager.h"
#include "Background.h"

DEF_PLUGIN_CREATOR(BackgroundManager)

void BackgroundManager::init(const std::list<IPlugin*> &plugins)
{
	for (auto plugin: plugins) {
		if (!setting) setting = dynamic_cast<ISetting *>(plugin);
		if (setting) break;
	}
}

IBackground* BackgroundManager::create(Json* jnode)
{
	return new Background(jnode, setting);
}
