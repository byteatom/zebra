#ifndef IMAINWND_H
#define IMAINWND_H

#include <QWidget>

#include "INode.h"
#include "INodeBox.h"

class IMainWnd
{
public:
	virtual QWidget* widgetAsMainWnd() = 0;
	virtual INode* currentNode(INode::Type type) = 0;
	/**
	 * Get type's nodebox, e.g., if SCREEN as nodebox wanted, type should be PROGRAM
	 */
	virtual INodeBox* currentNodeBox(INode::Type type) = 0;
	virtual void addNode(INode* node) = 0;
	virtual void setCurrentNode(INode* node) = 0;
	virtual void activateView(QWidget* child) = 0;
	virtual void activateCfger(QWidget* child) = 0;
protected:
	virtual ~IMainWnd() = default;
};

#endif // IMAINWND_H
