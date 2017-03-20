#ifndef IBORDER_H
#define IBORDER_H

#include <QPaintDevice>

#include "INodePart.h"
#include "IProjJson.h"
#include "ICoreJson.h"
#include "IAnimate.h"

class IBorder : public INodePart, public IProjJson, public ICoreJson, public IAnimate
{
public:
	virtual void setSurface(QPaintDevice* surface) = 0;
	virtual int width() = 0;

	virtual ~IBorder() = default;
};

#endif // IBORDER_H
