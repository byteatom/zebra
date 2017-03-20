#ifndef UDPFACTORY_H
#define UDPFACTORY_H

#include "IPlugin.h"
#include "ICarrierFactory.h"

class UdpFactory : public IPlugin, public ICarrierFactory
{
	virtual ~UdpFactory() = default;

	virtual ICarrier::TypeName typeName() override;
	virtual ICarrier* create(Json* jnode) override;
};

#endif // UDPFACTORY_H
