#ifndef IANIMATABLE_H
#define IANIMATABLE_H

class IAnimate
{
public:
	virtual void play() = 0;
	virtual void draw() = 0;
	virtual void stop() = 0;
protected:
	virtual ~IAnimate() = default;
};

#endif // IANIMATABLE_H
