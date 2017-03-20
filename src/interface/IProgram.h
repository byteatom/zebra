#ifndef IPROGRAM_H
#define IPROGRAM_H

#include "IAnimate.h"

class IProgram : public IAnimate
{
protected:
	virtual ~IProgram() = default;
};

#endif // IPROGRAM_H
