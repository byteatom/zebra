#include "ProgramAttr.h"

#include "INode.h"

ProgramAttr::ProgramAttr(INodeBox* box, Json* jnode, IBackground* background, IBorder* border):
	NodeAttr{box, INode::Type::PROGRAM, jnode},
	contentRect{&contentRect},
	background{background},	border{border}
{
	contentRect = box->contentRect();
	auto slot = [this](){updateContentRect();};
	boxRectConn = box->contentRect()->size.connect(slot);
	if (border) border->cfgChanged(slot);

	if (jnode) {
		jtov(mode);
		switch (mode) {
		case Mode::SEQUENTIAL:
			break;
		case Mode::SPECIFIED:
			if (jnode->find("start") != jnode->end())
				start = QDateTime::fromString((*jnode)["start"], Qt::ISODate);
			if (jnode->find("stop") != jnode->end())
				stop = QDateTime::fromString((*jnode)["stop"], Qt::ISODate);
			jtov(monday);
			jtov(tuesday);
			jtov(wednesday);
			jtov(thursday);
			jtov(friday);
			jtov(saturday);
			jtov(sunDay);
			break;
		case Mode::FIXED:
			jtov(fixedValue);
			break;
		case Mode::EXTERNAL:
			jtov(externalValue);
			break;
		default:
			break;
		}
	}
}

ProgramAttr::~ProgramAttr()
{
	boxRectConn.disconnect();
	if (background) delete background;
	if (border) delete border;
}

void ProgramAttr::updateContentRect()
{
	int borderWidth = border ? border->width() : 0;
	QSize boxSize= box->contentRect()->size;

	contentRect->setRect(borderWidth, borderWidth,
						 boxSize.width() - borderWidth * 2,
						 boxSize.height() - borderWidth * 2);
}

void ProgramAttr::exportProjJson(Json& jnode)
{
	NodeAttr::exportProjJson(jnode);

	vtoj(mode);
	switch (mode) {
	case Mode::SEQUENTIAL:
		break;
	case Mode::SPECIFIED:
		jnode["start"] = start.toString(Qt::ISODate);
		jnode["stop"] = stop.toString(Qt::ISODate);
		vtoj(monday);
		vtoj(tuesday);
		vtoj(wednesday);
		vtoj(thursday);
		vtoj(friday);
		vtoj(saturday);
		vtoj(sunDay);
		break;
	case Mode::FIXED:
		vtoj(fixedValue);
		break;
	case Mode::EXTERNAL:
		vtoj(externalValue);
		break;
	default:
		break;
	}

	IProjJson* backgroundJson = dynamic_cast<IProjJson*>(background);
	if (backgroundJson) backgroundJson->exportProjJson(jnode);

	IProjJson* borderJson = dynamic_cast<IProjJson*>(border);
	if (borderJson) borderJson->exportProjJson(jnode);
}

QString ProgramAttr::exportCoreJson(Json& jnode)
{
	static const QStr modeStr[ProgramAttr::MODE_MAX] {
		"sequence",
		"fixDate",
		"fixLength",
		"external"
	};

	char week[8];
	week[0] = sunDay ? '1' : '0';
	week[1] = monday ? '1' : '0';
	week[2] = tuesday ? '1' : '0';
	week[3] = wednesday ? '1' : '0';
	week[4] = thursday ? '1' : '0';
	week[5] = friday ? '1' : '0';
	week[6] = saturday ? '1' : '0';
	week[7] = 0;

	jnode["name"] = "none";
    if (mode == SPECIFIED) {
	jnode["playControl"] = {
		{"timeMode", modeStr[mode]},
		{"cmpDate", true},
		{"cmpTime", true},
		{"beginTime", {
				{"date", start.date().toString(Qt::ISODate)},
				{"week", week},
				{"time", start.time().toString(Qt::ISODate)}
			}
		},
		{"endTime", {
				{"date", stop.date().toString(Qt::ISODate)},
				{"week", week},
				{"time", stop.time().toString(Qt::ISODate)}
			}
		}
	};
    } else if (mode == FIXED) {
        jnode["playControl"] = {
            {"timeMode", modeStr[mode]},
            {"timeLong", fixedValue},
        };
    } else {
        jnode["playControl"] = {
            {"timeMode", modeStr[mode]},
            {"timeLong", 0},
        };
    }

	ICoreJson* backgroundJson = dynamic_cast<ICoreJson*>(background);
	if (backgroundJson) {
		QString ret = backgroundJson->exportCoreJson(jnode);
		if (!ret.isEmpty()) return ret;
	}

	ICoreJson* borderJson = dynamic_cast<ICoreJson*>(border);
	if (borderJson) {
		QString ret = borderJson->exportCoreJson(jnode);
		if (!ret.isEmpty()) return ret;
	}

	return QString();
}
