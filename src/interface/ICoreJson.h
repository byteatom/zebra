#ifndef ICOREJSON_H
#define ICOREJSON_H

#include "Json.h"

class ICoreJson
{
public:
	virtual QString exportCoreJson(Json& jnode /**<json object from INode*/) = 0;
protected:
	virtual ~ICoreJson() = default;
};

#endif // ICOREJSON_H
