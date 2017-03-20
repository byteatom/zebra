#ifndef IGLOW_H
#define IGLOW_H

#include <QPaintDevice>

#include "INodePart.h"
#include "IProjJson.h"
#include "ICoreJson.h"
#include "IAnimate.h"

class IGlow : public INodePart, public IProjJson, public ICoreJson, public IAnimate
{
public:
	virtual void setSurface(QPaintDevice* surface) = 0;

	virtual ~IGlow() = default;
};

#endif // IGLOW_H
