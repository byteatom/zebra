#ifndef INODEBOX_H
#define INODEBOX_H

#include <memory>

#include <QWidget>
#include <QString>

#include "AttrRect.h"

class INode;

class INodeBox
{
public:
	virtual void append(INode* child) = 0;
	virtual void remove(INode* child) = 0;
	virtual void raise(INode* child) = 0;
	virtual void lower(INode* child) = 0;
	virtual INode* next(std::shared_ptr<void>& pos) = 0;
	virtual void clear() = 0;
	virtual QString buildName(const QString& prefix) = 0;
	virtual bool checkName(INode* child, const QString& name) = 0;
	virtual void activateView(QWidget* child) = 0;
	virtual void activateCfger(QWidget* child) = 0;
	virtual Attr<int>& colorMode() = 0;
	virtual Attr<KRect>& contentRect() = 0;
protected:
	virtual ~INodeBox() = default;
};

#endif // INODEBOX_H
