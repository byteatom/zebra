#include "Fade.h"

#include "FadeCfger.h"

Fade::Fade(Json* jnode, int defaultStyle, bool hopsDisabled):
	style{defaultStyle}, hopsDisabled{hopsDisabled}
{
	if (jnode) {
		jtov2(style, "fadeStyle");
		jtov2(speed, "fadeSpeed");
		jtov2(stay, "fadeStay");
		jtov2(hops, "fadeHops");
	}
}

QWidget* Fade::newCfger()
{
	return new FadeCfger(this);
}

void Fade::cfgChanged(const std::function<void ()>& slot)
{

}

void Fade::exportProjJson(Json& jnode)
{
	vtoj2(style, "fadeStyle");
	vtoj2(speed, "fadeSpeed");
	vtoj2(stay, "fadeStay");
	vtoj2(hops, "fadeHops");
}

QString Fade::exportCoreJson(Json& jnode)
{
	static struct StyleCore {
		const FadeStyle style;
		const int core;
	} styleCfg[] = {
		{FadeRandom, 0},
		{FadeStatic, 1},
		{FadeMoveRight, 2},
		{FadeMoveLeft, 3},
		{FadeMoveDown, 4},
		{FadeMoveUp, 5},
		{FadeOverlayRight, 6},
		{FadeOverlayLeft, 7},
		{FadeOverlayDown, 8},
		{FadeOverlayUp, 9},
		{FadeOpenHorizontal, 10},
		{FadeOpenVertical, 11},
		{FadeCloseHorizontal, 12},
		{FadeCloseVertical, 13},
		{FadeOutHorizontal, 14},
		{FadeOutVertical, 15},
		{FadeShutterDown, 16},
		{FadeShutterUp, 17},
		{FadeBlink, 18},
		{FadeOutRight, 19},
		{FadeOutLeft, 20},
		{FadeOutDown, 21},
		{FadeOutUp, 22},
		{FadePageLeft, 23},
		{FadePageRight, 24},
		{FadeOpenFan, 25},
		{FadeCloseFan, 26},
		{FadeSpinLeft, 27},
		{FadeSpinRight, 28},
		{FadePullDown, 29},
		{FadePullUp, 30},
		{FadeRollDown, 31},
		{FadeRollRight, 32},
		{FadeGrid, 33},
		{FadeContrastBlink, 34},
		{FadeExpandRectangle, 35},
		{FadeShrinkRectangle, 36},
		{FadePullRight, 37},
		{FadePullLeft, 38},
		{FadeExpandCross, 39},
		{FadeShrinkCross, 40},
		{FadeRectangleTopLeft, 41},
		{FadeRectangleTopRight, 42},
		{FadeRectangleBottomLeft, 43},
		{FadeRectangleBottomRight, 44},
		{FadeDiagonalTopLeft, 45},
		{FadeDiagonalTopRight, 46},
		{FadeDiagonalBottomLeft, 47},
		{FadeDiagonalBottomRight, 48},
		{FadeSolidDown, 49},
		{FadeSolidUp, 50},
		{FadeMoveLeftContinuous, 51},
		{FadeMoveUpContinuous, 52},
		{FadeMoveRightContinuous, 53},
		{FadeMoveDownContinuous, 54}
	};

	int core = 0;
	for (int i = 0; i < std::extent<decltype(styleCfg)>::value; ++i) {
		if (styleCfg[i].style == style) {
			core = styleCfg[i].core;
		}
	}
	vtoj2(core, "enterMode");
	vtoj2(speed, "speed");
	vtoj2(stay, "delay");
	vtoj2(hops, "jumpGrid");
	return QString();
}

void Fade::play()
{

}

void Fade::draw()
{

}

void Fade::stop()
{

}

void Fade::setMaterial(std::vector<QPixmap> materials)
{

}

void Fade::setSurface(QPaintDevice* surface)
{

}

bool Fade::continuous()
{
	return style >= FadeMoveLeftContinuous;
}

