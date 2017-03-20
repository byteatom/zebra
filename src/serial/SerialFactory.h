#ifndef SERIAL_FACTORY_H
#define SERIAL_FACTORY_H

#include "IPlugin.h"
#include "ICarrierFactory.h"

class SerialFactory : public IPlugin, public ICarrierFactory
{
	virtual ~SerialFactory() = default;

	virtual ICarrier::TypeName typeName() override;
	virtual ICarrier* create(Json* jnode) override;
};

#endif // SERIAL_FACTORY_H
