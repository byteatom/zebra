#include "SerialFactory.h"

DEF_PLUGIN_CREATOR(SerialFactory)

#include "Serial.h"

ICarrier::TypeName SerialFactory::typeName()
{
	return Serial::type_name();
}

ICarrier* SerialFactory::create(Json* jnode)
{
	return new Serial(jnode);
}
