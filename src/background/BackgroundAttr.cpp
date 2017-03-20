#include "BackgroundAttr.h"

BackgroundAttr::BackgroundAttr(ISetting* setting_) :
	setting(setting_)
{
	memset(&input, 0, sizeof input);
	memset(&output, 0, sizeof output);
	select = 0;
	style = 0;
	speed = 5;
	delay = 0;
}

BackgroundAttr::BackgroundAttr(Json& jnode, ISetting* setting_) :
	setting(setting_)
{
	memset(&input, 0, sizeof input);
	memset(&output, 0, sizeof output);
	select = 0;
	style = 0;
	speed = 5;
	delay = 0;

	Json jsons;
	jtov3(jsons, (&jnode), "background");
	if (jsons.is_object()) {
		jtov3(checked, (&jsons), "checked");
		jtov3(select, (&jsons), "select");
		jtov3(style, (&jsons), "style");
		jtov3(speed, (&jsons), "speed");
		jtov3(delay, (&jsons), "delay");
	}
}

BackgroundAttr::~BackgroundAttr()
{
}

void BackgroundAttr::exportProjJson(Json& jnode)
{
	Json jnodes;
	jnodes["checked"] = checked;
	jnodes["select"] = select;
	jnodes["style"] = style;
	jnodes["speed"] = speed;
	jnodes["delay"] = delay;
	jnode["background"] = jnodes;
}

QString BackgroundAttr::exportCoreJson(Json& jnode)
{
	if (!checked) return QString();

	toInputAttr(KG_CREATE_FILE);
	freeBackground(&output);
	backgroundWork(&input, &output);

	Json jbk;
	jbk["name"] = "none";
	jbk["frameTotal"] = output.frameTotal;
	jbk["elementTotal"] = output.elementTotal;
	jbk["pictureTotal"] = output.pictureTotal;
	jbk["frameSpeed"] = input.speed;

	for (int i = 0; i < output.elementTotal; ++i) {
		Json js;
		for (int j = 0; j < output.elementPtr[i].locusTotal; ++j) {
			js[j] = {
				{"pictureNumber", output.elementPtr[i].locusPtr[j].pictureNumber},
				{"offsetX", output.elementPtr[i].locusPtr[j].offsetX},
				{"offsetY", output.elementPtr[i].locusPtr[j].offsetY},
				{"nextPicture", output.elementPtr[i].locusPtr[j].nextPic},
				{"recycleTotal", output.elementPtr[i].locusPtr[j].recycleTotal},
			};
		}

		jbk["elementArray"][i] = {
			{"locusTotal", output.elementPtr[i].locusTotal},
			{"startX", output.elementPtr[i].startX},
			{"startY", output.elementPtr[i].startY},
			{"startPictureNumber", output.elementPtr[i].startPictureNumber},
			{"locusArray", js},
		};
	}

	for (int i = 0; i < output.pictureTotal; ++i) {
		jbk["pictureArray"][i] = {
			{"width", output.picturePtr[i].width},
			{"height", output.picturePtr[i].height},
			{"content", output.picturePtr[i].pictureContent},
		};
	}

	jnode["shineBackground"] = jbk;

	return QString();
}

void BackgroundAttr::toInputAttr(KGCreateMode mode)
{
	input.coord.x = 0;
	input.coord.y = 0;
	input.coord.width = sight->width();
	input.coord.height = sight->height();
	strcpy(input.dir, (setting->tempDir() + "/").toStdString().c_str());
	input.createMode = mode;
	input.drawPicType = KG_BMP_32BIT;
	input.style = style;
	input.speed = speed;
}

