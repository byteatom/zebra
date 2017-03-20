#ifndef DISCOVERMANAGER_H
#define DISCOVERMANAGER_H

#include <QObject>
#include <deque>
#include "IPlugin.h"
#include "ICommand.h"
#include "IInit.h"

#include "ICarrier.h"
#include "IMainWnd.h"
#include "IProtocol.h"

class DiscoverManager : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~DiscoverManager() = default;

	virtual Commands commands() override;
	virtual void init(const std::list<IPlugin*> &plugins) override;

	IMainWnd *mainWnd = nullptr;
};

#endif // DISCOVERMANAGER_H
