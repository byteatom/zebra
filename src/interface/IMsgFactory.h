#ifndef IMESSAGEFACTORY_H
#define IMESSAGEFACTORY_H

#include "IMsg.h"

class IMsgFactory
{
public:
	virtual IMsgSp create(size_t size) = 0;

	virtual ~IMsgFactory() = default;
};

#endif // IMESSAGEFACTORY_H
