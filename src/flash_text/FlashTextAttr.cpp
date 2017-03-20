#include "FlashTextAttr.h"

#include <string.h>

#include "INode.h"
#include "ISetting.h"

extern ISetting* setting;

FlashTextAttr::FlashTextAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::FLASH_TEXT, jnode}
{
	memset(&input, 0, sizeof input);
	memset(&output, 0, sizeof output);

	style = 0;
	speed = 5;
	fontSize = 12;
	fontColor = 0;
	bold = false;
	italic = false;
	underline = false;
	text.clear();
	html.clear();
	fontName.clear();

	if (jnode) {
		jtov(style);
		jtov(speed);
		jtov(fontSize);
		jtov(fontColor);
		jtov(bold);
		jtov(italic);
		jtov(underline);
		jtov(text);
		jtov(html);
		jtov(fontName);
	}
}

FlashTextAttr::~FlashTextAttr()
{
}

void FlashTextAttr::toInputAttr(KGCreateMode mode, const char *dir)
{
	int borderOccupy = border ? border->width() : 0;
	input.coord.x = borderOccupy;
	input.coord.y = borderOccupy;
	input.coord.width = geometry->size->width() - 2*borderOccupy;
	input.coord.height = geometry->size->height() - 2*borderOccupy;
	if (input.coord.width < 0) input.coord.width = 0;
	if (input.coord.height < 0) input.coord.height = 0;

	strcpy(input.dir, dir);
	input.createMode = mode;
	input.drawPicType = KG_BMP_32BIT;
	strcpy(input.fontFile,
		   QString("font/").append(fontName).toStdString().c_str());
	input.style = style;
	input.speed = speed;
	input.fontSize = fontSize;
	strcpy(input.text, text.toStdString().c_str());
	input.bold = bold ? 1 : 0;
	input.italic = italic ? 1 : 0;
	input.underline = underline ? 1 : 0;

	if (fontColor <= KG_COLOR_WHITE) {
		input.colorMode = FONT_COLOR_MODE_COLOR;
		input.colorCount = 1;
		input.colors[0] = (KGColor)fontColor;
	} else if (fontColor == KG_COLOR_RANDOM) {
		input.colorMode = FONT_COLOR_MODE_RANDOM;
		input.colorCount = 3;
		input.colors[0] = KG_COLOR_RED;
		input.colors[1] = KG_COLOR_GREEN;
		input.colors[2] = KG_COLOR_YELLOW;
		input.colors[3] = KG_COLOR_BLUE;
		input.colors[4] = KG_COLOR_CYAN;
		input.colors[5] = KG_COLOR_MAGENTA;
		input.colors[6] = KG_COLOR_WHITE;
	}
}

void FlashTextAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(style);
	vtoj(speed);
	vtoj(fontSize);
	vtoj(fontColor);
	vtoj(bold);
	vtoj(italic);
	vtoj(underline);
	vtoj(text);
	vtoj(html);
	vtoj(fontName);
}

QString FlashTextAttr::exportCoreJsonArea(Json& jArea)
{
	toInputAttr(KG_CREATE_FILE, setting->tempDir().toStdString().c_str());

	freeShineFontArea(&output);
	shineFontAreaWork(&input, &output);

	jArea["name"] = "none";
	jArea["frameTotal"] = output.frameTotal;
	jArea["elementTotal"] = output.elementTotal;
	jArea["pictureTotal"] = output.pictureTotal;
	jArea["frameSpeed"] = input.speed;

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

		jArea["elementArray"][i] = {
			{"locusTotal", output.elementPtr[i].locusTotal},
			{"startX", output.elementPtr[i].startX},
			{"startY", output.elementPtr[i].startY},
			{"startPictureNumber", output.elementPtr[i].startPictureNumber},
			{"locusArray", js},
		};
	}

	for (int i = 0; i < output.pictureTotal; ++i) {
		jArea["pictureArray"][i] = {
			{"width", output.picturePtr[i].width},
			{"height", output.picturePtr[i].height},
			{"content", output.picturePtr[i].pictureContent},
		};
	}

	return QString();
}
