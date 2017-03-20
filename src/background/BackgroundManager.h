#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ISetting.h"
#include "IBackgroundFactory.h"

class BackgroundManager : public QObject, public IPlugin, public IInit,
					public IBackgroundFactory
{
	Q_OBJECT

	virtual ~BackgroundManager() = default;

	virtual void init(const std::list<IPlugin*> &plugins) override;
	virtual IBackground* create(Json* jnode) override;

	ISetting *setting = nullptr;
};

#endif // BACKGROUNDMANAGER_H
