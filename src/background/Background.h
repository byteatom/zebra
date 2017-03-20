#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "BackgroundCfger.h"
#include "BackgroundAttr.h"
#include "ISetting.h"
#include "IBackground.h"

struct BackgroundData
{
	explicit BackgroundData(Json* jnode, ISetting* setting);
	~BackgroundData();

	BackgroundAttr* attr;
	BackgroundCfger* cfger;
};

struct ElementData {
    int startX;
    int startY;
    int picNum;
    int locusNum;
    int cycleCnt;
};

class Background : private BackgroundData, public IBackground
{
public:
	explicit Background(Json* jnode, ISetting *setting);

private:
	virtual ~Background() = default;

	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jnode) override;

	virtual void setSurface(QPaintDevice* surface) override;
	virtual void play() override;
	virtual void draw() override;
	virtual void stop() override;

	virtual void cfgChanged(const std::function<void ()>& slot) override;
	virtual QWidget* newCfger() override;

    using BackgroundData::attr;
    using BackgroundData::cfger;

    ISetting *setting;
    BackgroundInput inputBK;
    BackgroundOutput outputBK;
    int curFrame = 0;
    int speedCnt = 0;
    ElementData *element = nullptr;
};

#endif // TEXT_H
