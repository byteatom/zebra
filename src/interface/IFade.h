#ifndef IFADE_H
#define IFADE_H

#include <vector>

#include <QPixmap>
#include <QPaintDevice>

#include "INodePart.h"
#include "IProjJson.h"
#include "ICoreJson.h"
#include "IAnimate.h"

class IFade : public INodePart, public IProjJson, public ICoreJson, public IAnimate
{
public:
	virtual void setMaterial(std::vector<QPixmap> materials) = 0;
	virtual void setSurface(QPaintDevice* surface) = 0;
	virtual bool continuous() = 0;

	virtual ~IFade() = default;
};

#endif // IFADE_H
