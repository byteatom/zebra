#ifndef CLOUD_H
#define CLOUD_H

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "IMainWnd.h"
#include "IProject.h"
#include "ISetting.h"

class Cluster : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~Cluster() = default;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;

	ISetting* setting = nullptr;
	IMainWnd* mainWnd = nullptr;
	IProject* project = nullptr;
};

#endif // CLOUD_H
