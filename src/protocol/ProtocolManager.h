#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

#include "IPlugin.h"
#include "IInit.h"
#include "IProtocolFactory.h"
#include "ICarrierFactory.h"
#include "Protocol.h"

class ProtocolManager : public IPlugin, public IInit, public IProtocolFactory
{
	virtual ~ProtocolManager() = default;
	virtual void init(const std::list<IPlugin*>& plugins) override;

	virtual IProtocol* create(Json* jnode) override;

	CarrierFactories carrierFactories{{nullptr}};
};

#endif // PROTOCOLMANAGER_H
