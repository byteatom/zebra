#include "BorderManager.h"
#include "Border.h"

DEF_PLUGIN_CREATOR(BorderManager)

void BorderManager::init(const std::list<IPlugin*> &plugins)
{
    for (auto plugin: plugins) {
        if (!mainWnd) mainWnd = dynamic_cast<IMainWnd *>(plugin);
        if (!setting) setting = dynamic_cast<ISetting *>(plugin);
        if (mainWnd && setting) break;
    }
}

IBorder* BorderManager::create(Json* jnode)
{
    return new Border(jnode, setting);
}
