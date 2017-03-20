#ifndef IPROJJSON_H
#define IPROJJSON_H

#include "Json.h"

class IProjJson
{
public:
	virtual void exportProjJson(Json& jnode /**<json object build from INode*/) = 0;
protected:
	virtual ~IProjJson() = default;
};

#endif // IPROJJSON_H
