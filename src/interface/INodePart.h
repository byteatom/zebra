#ifndef INODEPART_H
#define INODEPART_H

#include <functional>

#include <QWidget>

class INodePart
{
public:
	/**
	 * Create a cfger
	 * @return caller's duty to free
	 */
	virtual QWidget* newCfger() = 0;

	virtual void cfgChanged(const std::function<void ()>& slot) = 0;

protected:
	virtual ~INodePart() = default;
};

#endif // INODEPART_H
