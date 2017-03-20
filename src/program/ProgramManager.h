#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"
#include "IMainWnd.h"
#include "IBackgroundFactory.h"
#include "IBorderFactory.h"

class ProgramManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~ProgramManager() = default;
	virtual Commands commands() override;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;

	IMainWnd* mainWnd = nullptr;
	IBackgroundFactory* backgroundFactory = nullptr;
	IBorderFactory* borderFactory = nullptr;
	INodeFactory* nodeFactories[INode::Type::MAX_TYPE] = {0};
};

#endif // PROGRAMMANAGER_H
