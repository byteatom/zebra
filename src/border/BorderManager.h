#ifndef BORDERMANAGER_H
#define BORDERMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "IMainWnd.h"
#include "ISetting.h"
#include "IBorderFactory.h"

class BorderManager : public QObject, public IPlugin, public IInit,
                    public IBorderFactory
{
	Q_OBJECT

    virtual ~BorderManager() = default;

	virtual void init(const std::list<IPlugin*> &plugins) override;
    virtual IBorder* create(Json* jnode) override;

    IMainWnd *mainWnd = nullptr;
    ISetting *setting = nullptr;
};

#endif // BORDERMANAGER_H
