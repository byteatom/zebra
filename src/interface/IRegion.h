#ifndef IREGION_H
#define IREGION_H

#include <functional>

#include <boost/signals2.hpp>

#include "IAnimate.h"
#include "AttrRect.h"
#include "Json.h"

class IRegion : public IAnimate
{
public:
	typedef std::function<void (int count, int current)> PageObserver;
	virtual boost::signals2::connection pageObserve(PageObserver observer) = 0;
	virtual void pageFirst() = 0;
	virtual void pagePrevious() = 0;
	virtual void pageNext() = 0;
	virtual void pageLast() = 0;
	virtual Attr<KRect>& geometry() = 0;
	virtual Attr<KRect>& contentRect() = 0;

protected:
	virtual ~IRegion() = default;
};

#endif // IREGION_H
