#include "ProtocolManager.h"

DEF_PLUGIN_CREATOR(ProtocolManager)

void ProtocolManager::init(const std::list<IPlugin*>& plugins)
{
	for (auto plugin: plugins) {
		ICarrierFactory* factory = dynamic_cast<ICarrierFactory *>(plugin);
		if (factory) {
			carrierFactories.at(factory->typeName().type) = factory;
		}
	}
}

IProtocol* ProtocolManager::create(Json* jnode)
{
	return new Protocol(carrierFactories, jnode);
}
