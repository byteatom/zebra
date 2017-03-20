#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"

class CalendarManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~CalendarManager() = default;
	virtual Commands commands() override;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;
};

#endif // CALENDARMANAGER_H
