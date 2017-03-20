#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"
#include "IMainWnd.h"
#include "IProtocolFactory.h"
#include "ICore.h"

class ScreenManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~ScreenManager() = default;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual Commands commands() override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;

	void zoomIn();
	void zoomOut();
	void zoomFit();

	IMainWnd* mainWnd = nullptr;
	ICore* core = nullptr;
	INodeFactory* nodeFactory = nullptr;
	IProtocolFactory* protocolFactory = nullptr;
};

#endif // SCREENMANAGER_H
