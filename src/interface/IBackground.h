#ifndef IBACKGROUND_H
#define IBACKGROUND_H

#include <QPaintDevice>

#include "INodePart.h"
#include "IProjJson.h"
#include "ICoreJson.h"
#include "IAnimate.h"

class IBackground : public INodePart, public IProjJson, public ICoreJson, public IAnimate
{
public:
	virtual void setSurface(QPaintDevice* surface) = 0;

	virtual ~IBackground() = default;
};

#endif // IBACKGROUND_H
