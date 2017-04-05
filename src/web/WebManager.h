#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"
#include "ISetting.h"

extern ISetting* setting;

class WebManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~WebManager() = default;
	virtual Commands commands() override;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;
};

#endif // WEBMANAGER_H
