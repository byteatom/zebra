#include "BorderAttr.h"
#include <QUuid>

BorderAttr::BorderAttr()
{
}

BorderAttr::BorderAttr(Json& jnode)
{
	Json jsons;
	jtov3(jsons, (&jnode), "waterEdging");
	if (jsons.is_object()) {
		jtov3(borderPath, (&jsons), "borderPath");
		jtov3(checked, (&jsons), "checked");
		jtov3(style, (&jsons), "style");
		jtov3(speed, (&jsons), "speed");
		jtov3(direct, (&jsons), "direct");
	}
}

BorderAttr::~BorderAttr()
{
}

void BorderAttr::exportProjJson(Json& jnode)
{
	Json jnodes;
	jnodes["checked"] = checked;
	jnodes["style"] = style;
	jnodes["speed"] = speed;
	jnodes["direct"] = direct;
	jnodes["borderPath"] = borderPath;
	jnode["waterEdging"] = jnodes;
}

QString BorderAttr::exportCoreJson(Json& jnode)
{
	if (!checked) return QString();

	QString uuid = QUuid::createUuid().toString();
	QString borderUp = createDir + "/" + uuid + "border_up.bmp";
	QString borderDown = createDir + "/" + uuid + "border_down.bmp";
	QString borderLeft = createDir + "/" + uuid + "border_left.bmp";
	QString borderRight = createDir + "/" + uuid + "border_right.bmp";
	Json jnodes;
	jnodes["width"] = pixUp.width();
	jnodes["height"] = pixUp.height();
	jnodes["speed"] = speed;
	if (direct%3 == 0)
		jnodes["mode"] = "static";
	else if (direct%3 == 1)
		jnodes["mode"] = "forward";
	else if (direct%3 == 2)
		jnodes["mode"] = "reversal";
	else
		jnodes["mode"] = "none";
	jnodes["flicker"] = direct >= 3 ? true : false;
	jnodes["pictureUpContent"] = borderUp;
	jnodes["pictureDownContent"] = borderDown;
	jnodes["pictureLeftContent"] = borderLeft;
	jnodes["pictureRightContent"] = borderRight;
	jnode["waterEdging"] = jnodes;

	pixUp.save(borderUp);
	pixDown.save(borderDown);
	pixLeft.save(borderLeft);
	pixRight.save(borderRight);

	return QString();
}

