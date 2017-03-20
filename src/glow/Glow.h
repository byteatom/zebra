#ifndef GLOW_H
#define GLOW_H

#include <boost/signals2.hpp>

#include "IGlow.h"
#include "AttrRect.h"
#include "GlowEffect.h"

class Glow : public IGlow
{
public:
	Glow(Json* jnode, Attr<KRect>* geometry, Attr<KRect>* contentRect);
private:
	virtual ~Glow() = default;

	virtual QWidget* newCfger() override;
	virtual void cfgChanged(const std::function<void ()>& slot) override;
	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jNodes) override;
	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;
	virtual void setSurface(QPaintDevice* surface) override;

	Attr<KRect>* geometry;
	Attr<KRect>* contentRect;

	GlowEffect effectBg;
	GlowEffectFg effectFg;

	boost::signals2::signal<void ()> changed;

	friend class GlowCfger;
};

#endif // GLOW_H
