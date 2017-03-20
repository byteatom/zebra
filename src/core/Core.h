#ifndef CORE_H
#define CORE_H

#include "IPlugin.h"
#include "ICore.h"

class Core : public IPlugin, public ICore
{
	virtual ~Core() = default;

	virtual bool build(const char* json) override;
};

#endif // CORE_H
