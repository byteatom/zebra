#ifndef SYNCTIMEMANAGER_H
#define SYNCTIMEMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "ICommand.h"
#include "IInit.h"
#include "IMainWnd.h"

class SyncTimeManager : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~SyncTimeManager() = default;

	virtual Commands commands() override;
	virtual void init(const std::list<IPlugin*> &plugins) override;

	IMainWnd *mainWnd = nullptr;
};

#endif // SYNCTIMEMANAGER_H
