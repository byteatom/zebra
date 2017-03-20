#ifndef FADE_H
#define FADE_H

#include "IFade.h"
#include "FadeStyle.h"

class Fade : public IFade
{
public:
	Fade(Json* jnode, int defaultStyle, bool hopsDisabled);
private:
	virtual ~Fade() = default;

	virtual QWidget* newCfger() override;
	virtual void cfgChanged(const std::function<void ()>& slot) override;
	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jnode) override;
	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;
	virtual void setMaterial(std::vector<QPixmap> materials) override;
	virtual void setSurface(QPaintDevice* surface) override;
	virtual bool continuous() override;

	int style;
	int speed = 5;
	int stay = 0;
	int hops = 0;
	bool hopsDisabled;

	friend class FadeCfger;
};

#endif // FADE_H
