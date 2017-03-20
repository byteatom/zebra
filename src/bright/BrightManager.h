#ifndef SCREENBRIGHTTMANAGER_H
#define SCREENBRIGHTTMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "ICommand.h"
#include "IInit.h"
#include "IMainWnd.h"

class BrightManager : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~BrightManager() = default;

	virtual Commands commands() override;
	virtual void init(const std::list<IPlugin*> &plugins) override;

	IMainWnd *mainWnd = nullptr;
};

#endif // SCREENBRIGHTMANAGER_H
