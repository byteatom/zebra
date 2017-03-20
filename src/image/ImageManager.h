#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "IInit.h"
#include "ICommand.h"
#include "INodeFactory.h"

class ImageManager : public QObject, public IPlugin, public IInit,
		public ICommand, public INodeFactory
{
	Q_OBJECT

	virtual ~ImageManager() = default;
	virtual Commands commands() override;

	virtual void init(const std::list<IPlugin*>& plugins) override;
	virtual INode::Type type() override;
	virtual void create() override;
	virtual INode* create(INodeBox* box, Json& jnode) override;
};

#endif // TEXTMANAGER_H
