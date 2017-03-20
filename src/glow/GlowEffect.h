#ifndef GLOWEFFECT_H
#define GLOWEFFECT_H

#include <list>

#include "QStr.h"

typedef std::list<QStr> FilePaths;

struct GlowEffect
{
	bool bg = true;
	bool enabled = false;
	int speed = 5;
	FilePaths paths;
};

struct GlowEffectFg : public GlowEffect
{
	GlowEffectFg();

	bool overlap = true;
};

#endif // GLOWEFFECT_H
