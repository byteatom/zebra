#ifndef SCREENONOFFMANAGER_H
#define SCREENONOFFMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "ICommand.h"
#include "IInit.h"
#include "IMainWnd.h"

class OnOffManager : public QObject, public IPlugin, public IInit, public ICommand
{
	Q_OBJECT

	virtual ~OnOffManager() = default;

	virtual Commands commands() override;
	virtual void init(const std::list<IPlugin*> &plugins) override;

	IMainWnd *mainWnd = nullptr;
};

#endif // SCREENONOFFMANAGER_H
