#ifndef BORDER_H
#define BORDER_H

#include "BorderCfger.h"
#include "BorderAttr.h"
#include "ISetting.h"
#include "IBorder.h"

struct BorderData
{
    explicit BorderData(Json* jnode);
    ~BorderData();

    BorderAttr* attr;
    BorderCfger* cfger;
};

class Border : private BorderData, public IBorder
{
public:
    explicit Border(Json* jnode, ISetting *setting);

private:
    virtual ~Border() = default;

	virtual void exportProjJson(Json& jnode) override;
	virtual QString exportCoreJson(Json& jnode) override;

    virtual void setSurface(QPaintDevice* surface) override;
    virtual int width() override;
    virtual void play() override;
    virtual void draw() override;
    virtual void stop() override;

    virtual void cfgChanged(const std::function<void ()>& slot) override;
    virtual QWidget* newCfger() override;

    using BorderData::attr;
    using BorderData::cfger;
};

#endif // TEXT_H
