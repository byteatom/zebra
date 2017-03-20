#ifndef IINIT_H
#define IINIT_H

#include <list>

#include "IPlugin.h"

class IInit
{
public:
	enum Priority {LANGUAGE, DEFUALT, MAINWND, PROJECT};

	virtual Priority priority() {return Priority::DEFUALT;}
	virtual void init(const std::list<IPlugin*>& plugins) = 0;
protected:
	virtual ~IInit() = default;
};

#endif // IINIT_H
