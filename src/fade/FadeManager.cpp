#include "FadeManager.h"

DEF_PLUGIN_CREATOR(FadeManager)

#include "Fade.h"

IFade* FadeManager::create(Json* jnode, int defaultStyle, bool hopsDisabled)
{
	return new Fade(jnode, defaultStyle, hopsDisabled);
}
