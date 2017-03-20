#ifndef INODE_H
#define INODE_H

#include <QWidget>

#include "IProjJson.h"
#include "ICoreJson.h"

class INodeBox;

class INodeData
{
public:
	virtual void activate() = 0;
	virtual ~INodeData() = default;
};

class INode : public IProjJson, public ICoreJson
{
public:
	enum Type {
		SCREEN,
		PROGRAM,
		TEXT,
		IMAGE,
		CLOCK,
		CALENDAR,
		TIMER,
		FLASH_TEXT,
		HUMITURE,
		VARIABLE,
		MAX_TYPE
	};

	virtual ~INode() = default;
	virtual INodeBox* box() = 0;
	virtual Type type() = 0;
	virtual QString typeName() = 0;
	virtual QString name() = 0;
	virtual void setName(QString& name) = 0;
	virtual INodeData* data() = 0;
	virtual void setData(INodeData* data) = 0;
	virtual QWidget* view() = 0;
	virtual QWidget* cfger() = 0;
	virtual void activate() = 0;
	virtual void deactive() = 0;
	virtual void zoom(qreal factor) = 0;
	virtual void userDelete() = 0;
};

#endif // INODE_H
