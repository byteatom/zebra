#include "ScreenAttr.h"

#include <QHash>

#include "INode.h"

ScreenAttr::ScreenAttr(INodeBox* box, Json* jnode):
	NodeAttr{box, INode::Type::SCREEN, jnode},
	contentRect{&contentRect}
{
	para.size.connect([this](){contentRect->size = para.size;});

	top.outFilePath = ".";
	top.privateModel  = "X4S";
	top.dataEndian = "big";
	top.colorThreshold = 128;

	led.model = "";
	led.change = true;
	led.baseColor = IScreen::BaseColor::ONE;
	led.moduleLines = 16;
	led.scanLines = 4;
	led.warpTimes = 4;
	led.warpBits = 8;
	led.warpMode = IScreen::WarpMode::DOWN;
	led.decodeMode = IScreen::DecodeMode::HIGH_138;
	led.dataPolarity = true;
	led.strPolarity = true;
	led.oePolarity = true;
	led.mirror = false;
	led.split = false;
	led.warpTurn = false;
	led.scanDot = false;
	led.moduleWidth = 32;
	led.moduleHeight = 16;
	led.emptyDot = 0;
	led.dotIndex = 0;
	led.emptyDotIndex = 0;
	led.color1 = 0;
	led.color2 = 0;
	led.color3 = 0;
	led.color4 = 0;
	led.expandRGB = false;
	led.complete = false;
	for (unsigned int i = 0; i < std::extent<decltype(led.dotMap)>::value; i++) {
		led.dotMap[i].x = i;
		led.dotMap[i].y = i;
		led.dotMap[i].empty = false;
	}
	for (unsigned int i = 0; i < std::extent<decltype(led.lineMap)>::value; i++) {
		led.lineMap[i] = i;
	}

	para.cardModel = "AUTO"/*"X4S"*/;
	para.ledModel = "";
	para.size = QSize{512, 256};
	para.cardSeries = IScreen::CardSeries::COLORFUL;
	para.dataPolarity = true;
	para.oePolarity = true;
	para.baseColor = IScreen::BaseColor::ONE;
	para.grayScale = IScreen::GrayScale::NONE;
	para.threshold = 120;
	para.lineVerify = 0;
	para.frequency = 2;
	para.lineBlank = 20;
	para.gama = 2.0;
	para.configIndex = 0;
	para.autoMode = true;
	para.seniorCfgHide = false;

	power.timerSwitch = false;
	power.openTime = QTime(8, 0, 0, 0);
	power.closeTime = QTime(23, 0, 0, 0);

	sync.manualSel = false;
	sync.systemSel = true;
	sync.relativeTime = false;
	sync.time = QTime{0,0,0,0};
	sync.date = QDate{2016,1,1};


	bright.manualSel = true;
	bright.timingSel = false;
	bright.autoSel = false;
	bright.manualBright = 16;
	bright.timingCheck[0] = true;
	bright.timingCheck[1] = true;
	bright.timingCheck[2] = true;
	bright.timingCheck[3] = true;
	bright.startTime[0] = QTime{0,0,0,0};
	bright.startTime[1] = QTime{5,0,0,0};
	bright.startTime[2] = QTime{19,0,0,0};
	bright.startTime[3] = QTime{22,0,0,0};
	bright.endTime[0] = QTime{5,0,0,0};
	bright.endTime[1] = QTime{19,0,0,0};
	bright.endTime[2] = QTime{22,0,0,0};
	bright.endTime[3] = QTime{23,59,59,0};
	bright.timingBright[0] = 5;
	bright.timingBright[1] = 16;
	bright.timingBright[2] = 12;
	bright.timingBright[3] = 8;
	bright.defaultBright = 16;

	if (jnode) {
		Json::iterator jit;
		Json::iterator jit2;

		jit = jnode->find("top");
		if (jit != jnode->end()) {
			jit2 = jit->find("outFilePath");
			if (jit2 != jit->end()) top.outFilePath = *jit2;

			jit2 = jit->find("privateModel");
			if (jit2 != jit->end()) top.privateModel = *jit2;

			jit2 = jit->find("dataEndian");
			if (jit2 != jit->end()) top.dataEndian = *jit2;

			jit2 = jit->find("colorThreshold");
			if (jit2 != jit->end()) top.colorThreshold = *jit2;
		}

		jit = jnode->find("led");
		if (jit != jnode->end()) {
			jit2 = jit->find("model");
			if (jit2 != jit->end()) led.model = *jit2;

			jit2 = jit->find("change");
			if (jit2 != jit->end()) led.change = *jit2;

			jit2 = jit->find("baseColor");
			if (jit2 != jit->end()) led.baseColor = *jit2;

			jit2 = jit->find("moduleLines");
			if (jit2 != jit->end()) led.moduleLines = *jit2;

			jit2 = jit->find("scanLines");
			if (jit2 != jit->end()) led.scanLines = *jit2;

			jit2 = jit->find("warpTimes");
			if (jit2 != jit->end()) led.warpTimes = *jit2;

			jit2 = jit->find("warpBits");
			if (jit2 != jit->end()) led.warpBits = *jit2;

			jit2 = jit->find("warpMode");
			if (jit2 != jit->end()) led.warpMode = *jit2;

			jit2 = jit->find("decodeMode");
			if (jit2 != jit->end()) led.decodeMode = *jit2;

			jit2 = jit->find("dataPolarity");
			if (jit2 != jit->end()) led.dataPolarity = *jit2;

			jit2 = jit->find("strPolarity");
			if (jit2 != jit->end()) led.strPolarity = *jit2;

			jit2 = jit->find("oePolarity");
			if (jit2 != jit->end()) led.oePolarity = *jit2;

			jit2 = jit->find("mirror");
			if (jit2 != jit->end()) led.mirror = *jit2;

			jit2 = jit->find("split");
			if (jit2 != jit->end()) led.split = *jit2;

			jit2 = jit->find("warpTurn");
			if (jit2 != jit->end()) led.warpTurn = *jit2;

			jit2 = jit->find("scanDot");
			if (jit2 != jit->end()) led.scanDot = *jit2;

			jit2 = jit->find("moduleWidth");
			if (jit2 != jit->end()) led.moduleWidth = *jit2;

			jit2 = jit->find("moduleHeight");
			if (jit2 != jit->end()) led.moduleHeight = *jit2;

			jit2 = jit->find("emptyDot");
			if (jit2 != jit->end()) led.emptyDot = *jit2;
			jit2 = jit->find("dotIndex");
			if (jit2 != jit->end()) led.dotIndex = *jit2;

			jit2 = jit->find("emptyDotIndex");
			if (jit2 != jit->end()) led.emptyDotIndex = *jit2;


			jit2 = jit->find("color1");
			if (jit2 != jit->end()) led.color1 = *jit2;

			jit2 = jit->find("color2");
			if (jit2 != jit->end()) led.color2 = *jit2;

			jit2 = jit->find("color3");
			if (jit2 != jit->end()) led.color3 = *jit2;

			jit2 = jit->find("color4");
			if (jit2 != jit->end()) led.color4 = *jit2;

			jit2 = jit->find("expandRGB");
			if (jit2 != jit->end()) led.expandRGB = *jit2;

			jit2 = jit->find("complete");
			if (jit2 != jit->end()) led.complete = *jit2;

			jit2 = jit->find("dotMap");
			if (jit2 != jit->end()) {
				int i = 0;
				for (Json::iterator jit3 = jit2->begin();
					 jit3 != jit2->end() && i < led.dotIndex;
					 ++jit3, ++i) {
					led.dotMap[i].x = jit3->at(0);
					led.dotMap[i].y = jit3->at(1);
					//led.dotMap[i].empty = jit3->at(2) ? true : false;
				}
			}

			jit2 = jit->find("lineMap");
			if (jit2 != jit->end()) {
				int i = 0;
				for (Json::iterator jit3 = jit2->begin();
					 jit3 != jit2->end()
						&& i < led.scanLines;
					 ++jit3, ++i) {
					led.lineMap[i] = *jit3;
				}
			}
		}

		jit = jnode->find("para");
		if (jit != jnode->end()) {
			jit2 = jit->find("cardModel");
			if (jit2 != jit->end()) para.cardModel = *jit2;

			jit2 = jit->find("ledModel");
			if (jit2 != jit->end()) para.ledModel = *jit2;

			QSize screenSize = para.size;

			jit2 = jit->find("screenWidth");
			if (jit2 != jit->end()) screenSize.setWidth(*jit2);

			jit2 = jit->find("screenHeight");
			if (jit2 != jit->end()) screenSize.setHeight(*jit2);

			para.size = screenSize;

			jit2 = jit->find("cardSeries");
			if (jit2 != jit->end()) para.cardSeries = *jit2;
			jit2 = jit->find("dataPolarity");
			if (jit2 != jit->end()) para.dataPolarity = *jit2;
			jit2 = jit->find("oePolarity");
			if (jit2 != jit->end()) para.oePolarity = *jit2;
			jit2 = jit->find("baseColor");
			if (jit2 != jit->end()) para.baseColor = *jit2;

			jit2 = jit->find("grayScale");
			if (jit2 != jit->end()) para.grayScale = *jit2;

			jit2 = jit->find("threshold");
			if (jit2 != jit->end()) para.threshold = *jit2;

			jit2 = jit->find("lineVerify");
			if (jit2 != jit->end()) para.lineVerify = *jit2;

			jit2 = jit->find("frequency");
			if (jit2 != jit->end()) para.frequency = *jit2;

			jit2 = jit->find("lineBlank");
			if (jit2 != jit->end()) para.lineBlank = *jit2;

			jit2 = jit->find("gama");
			if (jit2 != jit->end()) para.gama = *jit2;

			jit2 = jit->find("configIndex");
			if (jit2 != jit->end()) para.configIndex = *jit2;
			jit2 = jit->find("autoMode");
			if (jit2 != jit->end()) para.autoMode = *jit2;

			jit2 = jit->find("seniorCfgHide");
			if (jit2 != jit->end()) para.seniorCfgHide = *jit2;
		}

		jit = jnode->find("power");
		if (jit != jnode->end()) {
			jit2 = jit->find("timerSwitch");
			if (jit2 != jit->end()) power.timerSwitch = *jit2;

			jit2 = jit->find("openTime");
			if (jit2 != jit->end()) power.openTime = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("closeTime");
			if (jit2 != jit->end()) power.closeTime = QTime::fromString(*jit2, Qt::ISODate);;
		}

		jit = jnode->find("sync");
		if (jit != jnode->end()) {
			jit2 = jit->find("manualSel");
			if (jit2 != jit->end()) sync.manualSel = *jit2;

			jit2 = jit->find("systemSel");
			if (jit2 != jit->end()) sync.systemSel = *jit2;

			jit2 = jit->find("time");
			if (jit2 != jit->end()) sync.time = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("date");
			if (jit2 != jit->end()) sync.date = QDate::fromString(*jit2, Qt::ISODate);
		}

		jit = jnode->find("bright");
		if (jit != jnode->end()) {
			jit2 = jit->find("manualSel");
			if (jit2 != jit->end()) bright.manualSel = *jit2;

			jit2 = jit->find("timingSel");
			if (jit2 != jit->end()) bright.timingSel = *jit2;

			jit2 = jit->find("autoSel");
			if (jit2 != jit->end()) bright.autoSel = *jit2;

			jit2 = jit->find("manualBright");
			if (jit2 != jit->end()) bright.manualBright = *jit2;

			jit2 = jit->find("timingCheck0");
			if (jit2 != jit->end()) bright.timingCheck[0] = *jit2;

			jit2 = jit->find("timingCheck1");
			if (jit2 != jit->end()) bright.timingCheck[1] = *jit2;

			jit2 = jit->find("timingCheck2");
			if (jit2 != jit->end()) bright.timingCheck[2] = *jit2;

			jit2 = jit->find("timingCheck3");
			if (jit2 != jit->end()) bright.timingCheck[3] = *jit2;

			jit2 = jit->find("startTime0");
			if (jit2 != jit->end()) bright.startTime[0] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("startTime1");
			if (jit2 != jit->end()) bright.startTime[1] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("startTime2");
			if (jit2 != jit->end()) bright.startTime[2] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("startTime3");
			if (jit2 != jit->end()) bright.startTime[3] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("endTime0");
			if (jit2 != jit->end()) bright.endTime[0] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("endTime1");
			if (jit2 != jit->end()) bright.endTime[1] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("endTime2");
			if (jit2 != jit->end()) bright.endTime[2] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("endTime3");
			if (jit2 != jit->end()) bright.endTime[3] = QTime::fromString(*jit2, Qt::ISODate);

			jit2 = jit->find("timingBright0");
			if (jit2 != jit->end()) bright.timingBright[0] = *jit2;

			jit2 = jit->find("timingBright1");
			if (jit2 != jit->end()) bright.timingBright[1] = *jit2;

			jit2 = jit->find("timingBright2");
			if (jit2 != jit->end()) bright.timingBright[2] = *jit2;

			jit2 = jit->find("timingBright3");
			if (jit2 != jit->end()) bright.timingBright[3] = *jit2;

			jit2 = jit->find("defaultBright");
			if (jit2 != jit->end()) bright.defaultBright = *jit2;
		}
	}
}

void ScreenAttr::exportProjJson(Json& jnode)
{
	NodeAttr::exportProjJson(jnode);

	jnode["top"] = {
		{"outFilePath", top.outFilePath},
		{"privateModel", top.privateModel},
		{"dataEndian", top.dataEndian},
		{"colorThreshold", top.colorThreshold}
	};

	Json jdotmap;
	for (unsigned int i = 0; i < led.dotIndex; i++) {
		jdotmap.push_back({led.dotMap[i].x, led.dotMap[i].y, led.dotMap[i].empty ? 1 : 0});
	}
	Json jlinemap;
	for (unsigned int i = 0; i < led.scanLines; i++) {
		jlinemap.push_back(led.lineMap[i]);
	}
	jnode["led"] = {
		{"model", led.model},
		{"change", led.change},
		{"baseColor", led.baseColor.get()},
		{"moduleLines", led.moduleLines},
		{"scanLines", led.scanLines},
		{"warpTimes", led.warpTimes},
		{"warpBits", led.warpBits},
		{"warpMode", led.warpMode},
		{"decodeMode", led.decodeMode},
		{"dataPolarity", led.dataPolarity},
		{"strPolarity", led.strPolarity},
		{"oePolarity", led.oePolarity},
		{"mirror", led.mirror},
		{"split", led.split},
		{"warpTurn", led.warpTurn},
		{"scanDot", led.scanDot},
		{"moduleWidth", led.moduleWidth},
		{"moduleHeight", led.moduleHeight},
		{"emptyDot", led.emptyDot},
		{"dotIndex", led.dotIndex},
		{"emptyDotIndex", led.emptyDotIndex},
		{"color1", led.color1},
		{"color2", led.color2},
		{"color3", led.color3},
		{"color4", led.color4},
		{"expandRGB", led.expandRGB},
		{"complete", led.complete},
		{"dotMap", jdotmap},
		{"lineMap", jlinemap},
	};

	jnode["para"] = {
		{"cardModel", para.cardModel},
		{"ledModel", para.ledModel},
		{"screenWidth", para.size->width()},
		{"screenHeight", para.size->height()},
		{"cardSeries", para.cardSeries},
		{"dataPolarity", para.dataPolarity},
		{"oePolarity", para.oePolarity},
		{"baseColor", para.baseColor},
		{"grayScale", para.grayScale},
		{"threshold", para.threshold},
		{"lineVerify", para.lineVerify},
		{"frequency", para.frequency},
		{"lineBlank", para.lineBlank},
		{"gama", para.gama},
		{"configIndex", para.configIndex},
		{"autoMode", para.autoMode},
		{"seniorCfgHide", para.seniorCfgHide},
	};

	jnode["power"] = {
		{"timerSwitch", power.timerSwitch},
		{"openTime", power.openTime.toString(Qt::ISODate)},
		{"closeTime", power.closeTime.toString(Qt::ISODate)},
	};

	jnode["sync"] = {
		{"manualSel", sync.manualSel},
		{"systemSel", sync.systemSel},
		{"time", sync.time.toString(Qt::ISODate)},
		{"date", sync.date.toString(Qt::ISODate)},
	};

	jnode["bright"] = {
		{"manualSel", bright.manualSel},
		{"timingSel", bright.timingSel},
		{"autoSel", bright.autoSel},
		{"manualBright", bright.manualBright},
		{"timingCheck0", bright.timingCheck[0]},
		{"timingCheck1", bright.timingCheck[1]},
		{"timingCheck2", bright.timingCheck[2]},
		{"timingCheck3", bright.timingCheck[3]},
		{"startTime0", bright.startTime[0].toString(Qt::ISODate)},
		{"startTime1", bright.startTime[1].toString(Qt::ISODate)},
		{"startTime2", bright.startTime[2].toString(Qt::ISODate)},
		{"startTime3", bright.startTime[3].toString(Qt::ISODate)},
		{"endTime0", bright.endTime[0].toString(Qt::ISODate)},
		{"endTime1", bright.endTime[1].toString(Qt::ISODate)},
		{"endTime2", bright.endTime[2].toString(Qt::ISODate)},
		{"endTime3", bright.endTime[3].toString(Qt::ISODate)},
		{"timingBright0", bright.timingBright[0]},
		{"timingBright1", bright.timingBright[1]},
		{"timingBright2", bright.timingBright[2]},
		{"timingBright3", bright.timingBright[3]},
		{"defaultBright", bright.defaultBright},
	};
}

const QHash<int, enum IScreen::BaseColor> baseColorJsonHash {
	{1, IScreen::BaseColor::ONE},
	{2, IScreen::BaseColor::TWO},
	{3, IScreen::BaseColor::THREE},
};

const QHash<QString, enum IScreen::WarpMode> warpModeJsonHash {
	{"upWarp", IScreen::WarpMode::UP},
	{"downWarp", IScreen::WarpMode::DOWN},
	{"straight", IScreen::WarpMode::STRAIGHT},
};

const QHash<QString, enum IScreen::DecodeMode> decodeModeJsonHash {
	{"high138", IScreen::DecodeMode::HIGH_138},
	{"low138", IScreen::DecodeMode::LOW_138},
	{"highIO", IScreen::DecodeMode::HIGH_IO},
	{"lowIO", IScreen::DecodeMode::LOW_IO},
};

const QHash<int, enum IScreen::GrayScale> grayScaleJsonHash {
	{0, IScreen::GrayScale::NONE},
	{4, IScreen::GrayScale::GRAY_4},
	{8, IScreen::GrayScale::GRAY_8},
	{16, IScreen::GrayScale::GRAY_16},
	{32, IScreen::GrayScale::GRAY_32},
	{256, IScreen::GrayScale::GRAY_256},
};

QString ScreenAttr::exportCoreJson(Json& jnode)
{
	top.privateModel = para.cardModel;
	top.dataEndian = (top.privateModel.compare("x4s", Qt::CaseInsensitive) == 0
					  || top.privateModel.compare("x4e", Qt::CaseInsensitive) == 0) ? "big" : "little";
	jnode["topLevelConfig"] = {
		{"outFilePath", top.outFilePath},
		{"privateModel", top.privateModel},
		{"dataEndian", top.dataEndian},
		{"colorThreshold", top.colorThreshold}
	};

	jnode["ledScreenConfig"] = {
			{"model", led.model},
			{"width", para.size->width()},
			{"height", para.size->height()},
			{"scanLine", led.moduleLines},
			{"scanMode", led.scanLines},
			{"baseColor", baseColorJsonHash.key((IScreen::BaseColor)led.baseColor.get())},
			{"warpBit", led.warpBits},
			{"warpMode", warpModeJsonHash.key((IScreen::WarpMode)led.warpMode)},
			{"abcdeMode", decodeModeJsonHash.key((IScreen::DecodeMode)led.decodeMode)},
			{"dataPolar", para.dataPolarity},
			{"strPolar", para.strPolarity},
			{"oePolar", para.oePolarity},
			{"screenMirror", led.mirror},
			{"screenSplit", led.split},
			{"grayLevel", grayScaleJsonHash.key((IScreen::GrayScale)para.grayScale)},
			{"gama", para.gama},
			{"lineVerify", para.lineVerify},
			{"frequency", para.frequency},
			{"lineBlank", para.lineBlank},
			{"configIndex", para.configIndex},
			{"extendInfo", led.scanDot},
		};

	if (led.scanDot) {
		Json jsonDotMap;
		int scanLen = (led.moduleWidth + led.emptyDot) * led.warpTimes;
		for (int j = 0; j < 256/scanLen; ++j) {
			for (int i = 0; i < scanLen; ++i) {
				jsonDotMap[j*scanLen + i] = {
					{"x", led.dotMap[i].x + j*led.moduleWidth},
					{"y", led.dotMap[i].y},
			};
		}
		}

		Json jsonLineMap;
		for (int j = 0; j < 16/led.scanLines; ++j) {
			for (int i = 0; i < led.scanLines; ++i) {
				if (i == 0) jsonLineMap[j*led.scanLines + i] = led.lineMap[led.scanLines - 1];
				else jsonLineMap[j*led.scanLines + i] = led.lineMap[i - 1];
			}
		}

		jnode["scanDotConfig"] = {
			{"model", led.model},
			{"width", led.moduleWidth},
			{"height", led.moduleHeight},
			{"scanLine", led.moduleLines},
			{"warpTimes", led.warpTimes},
			{"emptyDot", led.emptyDot},
			{"baseColor", baseColorJsonHash.key((IScreen::BaseColor)led.baseColor.get())},
			{"abcdeMode", decodeModeJsonHash.key((IScreen::DecodeMode)led.decodeMode)},
			{"dataPolar", para.dataPolarity},
			{"strPolar", para.strPolarity},
			{"oePolar", para.oePolarity},
			{"dotMap", jsonDotMap},
			{"lineMap", jsonLineMap},
		};
	}

	QDateTime dateTime;
	if (sync.systemSel)
		dateTime = QDateTime::currentDateTime();
	else {
		dateTime.setDate(sync.date);
		dateTime.setTime(sync.time);
	}


	jnode["displayConfig"] = {
		{"enAutoBright", bright.autoSel},
		{"updateBright", true},
		{"updateTime", true},
		{"enRelativeTime", sync.relativeTime},
		{"enAutoOpenScreen", power.timerSwitch},
		{"enAutoCloseScreen", power.timerSwitch},
		{"updateTimingBright", true},
		{"bright", bright.manualBright},
		{"openScreenTime", {
				{"cmpYear", false},
				{"cmpMonth", false},
				{"cmpDay", false},
				{"cmpHour", true},
				{"cmpMin", true},
				{"cmpSec", true},
				{"cmpSUN", true},
				{"cmpMON", true},
				{"cmpTUE", true},
				{"cmpWED", true},
				{"cmpTHU", true},
				{"cmpFRI", true},
				{"cmpSAT", true},
				{"time", {
						{"date", QString("%1-%2-%3").arg(0).arg(0).arg(0)},
						{"week", 0},
						{"time", QString("%1:%2:%3").arg(power.openTime.hour()) \
													.arg(power.openTime.minute()) \
													.arg(power.openTime.second())},
					},
				},
			},
		},
		{"closeScreenTime", {
				{"cmpYear", false},
				{"cmpMonth", false},
				{"cmpDay", false},
				{"cmpHour", true},
				{"cmpMin", true},
				{"cmpSec", true},
				{"cmpSUN", true},
				{"cmpMON", true},
				{"cmpTUE", true},
				{"cmpWED", true},
				{"cmpTHU", true},
				{"cmpFRI", true},
				{"cmpSAT", true},
				{"time", {
						{"date", QString("%1-%2-%3").arg(0).arg(0).arg(0)},
						{"week", 0},
						{"time", QString("%1:%2:%3").arg(power.closeTime.hour()) \
													.arg(power.closeTime.minute()) \
													.arg(power.closeTime.second())},
					},
				},
			},
		},
		{"currentTime", {
				{"date", QString("%1-%2-%3").arg(dateTime.date().year()) \
											.arg(dateTime.date().month()) \
											.arg(dateTime.date().day())},
				{"week", dateTime.date().dayOfWeek()},
				{"time", QString("%1:%2:%3").arg(dateTime.time().hour()) \
											.arg(dateTime.time().minute()) \
											.arg(dateTime.time().second())},
			},
		},
	};
	Json brightArray;
	for (int i = 0; i < 4; ++i) {
		brightArray[i] = {
			{"tBright", bright.timingBright[i]},
			{"beginTime", QString("%1:%2:%3").arg(bright.startTime[i].hour()) \
											 .arg(bright.startTime[i].minute()) \
											 .arg(bright.startTime[i].second())},
			{"endTime", QString("%1:%2:%3").arg(bright.endTime[i].hour()) \
										   .arg(bright.endTime[i].minute()) \
										   .arg(bright.endTime[i].second())},
		};
	}
	if (bright.timingSel) {
		jnode["displayConfig"]["timingBright"] = {
			{"defaultBright", bright.defaultBright},
				{"tBrightArray", brightArray},
		};
	}

	return QString();
}
