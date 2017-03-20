#include "UdpFactory.h"

DEF_PLUGIN_CREATOR(UdpFactory)

#include "Udp.h"

ICarrier::TypeName UdpFactory::typeName()
{
	return Udp::type_name();
}

ICarrier* UdpFactory::create(Json* jnode)
{
	return new Udp(jnode);
}
