#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"

class ClockManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~ClockManager() = default;
	virtual Commands commands() override;

	virtual void init(const std::list<IPlugin*> &plugins) override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;
};

#endif // CLOCKMANAGER_H
