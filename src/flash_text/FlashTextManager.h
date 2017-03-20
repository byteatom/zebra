#ifndef FLASHTEXTMANAGER_H
#define FLASHTEXTMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"

class FlashTextManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~FlashTextManager() = default;
	virtual Commands commands() override;

	virtual void init(const std::list<IPlugin*> &plugins) override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;
};

#endif // FLASHTEXTMANAGER_H
