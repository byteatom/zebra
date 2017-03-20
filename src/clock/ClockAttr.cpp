#include "ClockAttr.h"

#include <string.h>

#include "INode.h"
#include "ISetting.h"

extern ISetting* setting;

ClockAttr::ClockAttr(INodeBox* box, Json* jnode):
	RegionAttr{box, INode::Type::CLOCK, jnode}
{
	memset(&input, 0, sizeof input);
	memset(&output, 0, sizeof output);

	if (jnode) {
		jtov_enum(dialAttrInfo.dialStyle);

		jtov(dialAttrInfo.dialTime.dialPoint.hourPoint.pointSize);
		jtov(dialAttrInfo.dialTime.dialPoint.hourPoint.pointColor);
		jtov_enum(dialAttrInfo.dialTime.dialPoint.hourPoint.pointStyle);
		jtov(dialAttrInfo.dialTime.dialPoint.tsnPoint.pointSize);
		jtov(dialAttrInfo.dialTime.dialPoint.tsnPoint.pointColor);
		jtov_enum(dialAttrInfo.dialTime.dialPoint.tsnPoint.pointStyle);
		jtov(dialAttrInfo.dialTime.dialPoint.minutePoint.pointSize);
		jtov(dialAttrInfo.dialTime.dialPoint.minutePoint.pointColor);
		jtov_enum(dialAttrInfo.dialTime.dialPoint.minutePoint.pointStyle);

		jtov(dialAttrInfo.dialTime.dialHand.hourHand.handSize);
		jtov(dialAttrInfo.dialTime.dialHand.hourHand.handColor);
		jtov_enum(dialAttrInfo.dialTime.dialHand.hourHand.handStyle);
		jtov(dialAttrInfo.dialTime.dialHand.minuteHand.handSize);
		jtov(dialAttrInfo.dialTime.dialHand.minuteHand.handColor);
		jtov_enum(dialAttrInfo.dialTime.dialHand.minuteHand.handStyle);
		jtov(dialAttrInfo.dialTime.dialHand.secondHand.handSize);
		jtov(dialAttrInfo.dialTime.dialHand.secondHand.handColor);
		jtov_enum(dialAttrInfo.dialTime.dialHand.secondHand.handStyle);

		jtov_enum(dialAttrInfo.dialTime.dialTimeDiff.deltType);
		jtov(dialAttrInfo.dialTime.dialTimeDiff.deltHour);
		jtov(dialAttrInfo.dialTime.dialTimeDiff.deltMinute);
		jtov(dialAttrInfo.dialTime.dialTimeDiff.deltSecond);
	}
}

ClockAttr::~ClockAttr()
{
}

void ClockAttr::toInputAttr(KGCreateMode mode, const char *dir)
{
	int borderOccupy = border ? border->width() : 0;

	input.coord.x = geometry->point->x() + borderOccupy;
	input.coord.y = geometry->point->y() + borderOccupy;
	input.coord.width = geometry->size->width() - 2*borderOccupy;
	input.coord.height = geometry->size->height() - 2*borderOccupy;
	if (input.coord.width < 0) input.coord.width = 0;
	if (input.coord.height < 0) input.coord.height = 0;

	strcpy(input.dir, dir);
	input.createMode = mode;
	input.dialPara.size.width = input.coord.width;
	input.dialPara.size.height = input.coord.height;
	input.dialPara.dialType = (DialType)dialAttrInfo.dialStyle;
	input.dialPara.scaleType = (ScaleType)dialAttrInfo.dialTime.dialPoint.tsnPoint.pointStyle;
	input.dialPara.drawPicType = KG_BMP_32BIT;
	input.dialPara.hourColor = (KGColor)dialAttrInfo.dialTime.dialPoint.hourPoint.pointColor;
	input.dialPara.quarterColor = (KGColor)dialAttrInfo.dialTime.dialPoint.minutePoint.pointColor;
	input.dialPara.crossColor = (KGColor)dialAttrInfo.dialTime.dialPoint.tsnPoint.pointColor;
	input.dialPara.hourSize = dialAttrInfo.dialTime.dialPoint.hourPoint.pointSize;
	input.dialPara.quarterSize = dialAttrInfo.dialTime.dialPoint.minutePoint.pointSize;
	input.dialPara.crossSize = dialAttrInfo.dialTime.dialPoint.tsnPoint.pointSize;

	float step = input.dialPara.size.width > input.dialPara.size.height
				? input.dialPara.size.height : input.dialPara.size.width;
	input.handPara[0].color = (KGColor)dialAttrInfo.dialTime.dialHand.hourHand.handColor;
	input.handPara[0].drawPicType = KG_BMP_32BIT;
	input.handPara[0].handType = (HandType)dialAttrInfo.dialTime.dialHand.hourHand.handStyle;
	input.handPara[0].size.width = dialAttrInfo.dialTime.dialHand.hourHand.handSize;
	input.handPara[0].size.height = step / 3 / 1.25;

	input.handPara[1].color = (KGColor)dialAttrInfo.dialTime.dialHand.minuteHand.handColor;
	input.handPara[1].drawPicType = KG_BMP_32BIT;
	input.handPara[1].handType = (HandType)dialAttrInfo.dialTime.dialHand.minuteHand.handStyle;
	input.handPara[1].size.width = dialAttrInfo.dialTime.dialHand.minuteHand.handSize;
	input.handPara[1].size.height = step / 3 / 0.95;

	input.handPara[2].color = (KGColor)dialAttrInfo.dialTime.dialHand.secondHand.handColor;
	input.handPara[2].drawPicType = KG_BMP_32BIT;
	input.handPara[2].handType = (HandType)dialAttrInfo.dialTime.dialHand.secondHand.handStyle;
	input.handPara[2].size.width = dialAttrInfo.dialTime.dialHand.secondHand.handSize;
	input.handPara[2].size.height = step / 3 / 0.75;
}

void ClockAttr::exportProjJson(Json& jnode)
{
	RegionAttr::exportProjJson(jnode);

	vtoj(dialAttrInfo.dialTime.dialPoint.hourPoint.pointSize);
	vtoj(dialAttrInfo.dialTime.dialPoint.hourPoint.pointColor);
	vtoj(dialAttrInfo.dialTime.dialPoint.hourPoint.pointStyle);
	vtoj(dialAttrInfo.dialTime.dialPoint.tsnPoint.pointSize);
	vtoj(dialAttrInfo.dialTime.dialPoint.tsnPoint.pointColor);
	vtoj(dialAttrInfo.dialTime.dialPoint.tsnPoint.pointStyle);
	vtoj(dialAttrInfo.dialTime.dialPoint.minutePoint.pointSize);
	vtoj(dialAttrInfo.dialTime.dialPoint.minutePoint.pointColor);
	vtoj(dialAttrInfo.dialTime.dialPoint.minutePoint.pointStyle);

	vtoj(dialAttrInfo.dialTime.dialHand.hourHand.handSize);
	vtoj(dialAttrInfo.dialTime.dialHand.hourHand.handColor);
	vtoj(dialAttrInfo.dialTime.dialHand.hourHand.handStyle);
	vtoj(dialAttrInfo.dialTime.dialHand.minuteHand.handSize);
	vtoj(dialAttrInfo.dialTime.dialHand.minuteHand.handColor);
	vtoj(dialAttrInfo.dialTime.dialHand.minuteHand.handStyle);
	vtoj(dialAttrInfo.dialTime.dialHand.secondHand.handSize);
	vtoj(dialAttrInfo.dialTime.dialHand.secondHand.handColor);
	vtoj(dialAttrInfo.dialTime.dialHand.secondHand.handStyle);

	vtoj(dialAttrInfo.dialTime.dialTimeDiff.deltType);
	vtoj(dialAttrInfo.dialTime.dialTimeDiff.deltHour);
	vtoj(dialAttrInfo.dialTime.dialTimeDiff.deltMinute);
	vtoj(dialAttrInfo.dialTime.dialTimeDiff.deltSecond);
}

QString ClockAttr::exportCoreJsonArea(Json& jArea)
{
	toInputAttr(KG_CREATE_FILE, setting->tempDir().toStdString().c_str());
	freeWatchArea(&output);
	watchAreaWork(&input, &output);

	QString tmp = (output.jetLag[0] == 0 ? "-" : "") +
			QString::number(output.jetLag[1]) + ":" +
			QString::number(output.jetLag[2]) + ":" +
			QString::number(output.jetLag[3]);
	jArea["jetLag"] = tmp;
	jArea["backgroundContent"] = output.dialContent;

	for (int i = 0; i < 60; ++i)
		jArea["hourHandArray"][i] = {
			{"startX", output.hourHand[i].coord.x},
			{"startY", output.hourHand[i].coord.y},
			{"width", output.hourHand[i].coord.width},
			{"height", output.hourHand[i].coord.height},
			{"content", output.hourHand[i].handContent},
		};

	for (int i = 0; i < 60; ++i)
		jArea["minHandArray"][i] = {
			{"startX", output.minHand[i].coord.x},
			{"startY", output.minHand[i].coord.y},
			{"width", output.minHand[i].coord.width},
			{"height", output.minHand[i].coord.height},
			{"content", output.minHand[i].handContent},
		};

	for (int i = 0; i < 60; ++i)
		jArea["secHandArray"][i] = {
			{"startX", output.secHand[i].coord.x},
			{"startY", output.secHand[i].coord.y},
			{"width", output.secHand[i].coord.width},
			{"height", output.secHand[i].coord.height},
			{"content", output.secHand[i].handContent},
		};

	return QString();
}

/*
QDir dir("c:/temp/");
dir.setFilter(QDir::Files);
int i, j=dir.count()-1;
for (i = 0; i <= j; ++i)
	dir.remove(dir[i]);
*/
