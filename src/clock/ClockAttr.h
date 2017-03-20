#ifndef CLOCKATTR_H
#define CLOCKATTR_H

#include <boost/signals2.hpp>

#include <QCoreApplication>
#include <QPixmap>

#include "QStr.h"
#include "RegionAttr.h"
#include "KGWatch.h"
#include "Graphic.h"

#define X_RATIO 0.50
#define Y_RATIO 0.80

typedef uint32_t DialColor;

typedef enum { DS_CIRCLE = 0, DS_ELLIPSE, DS_RHOMBUS, DS_ROUNDRECT, DS_WINDMILL } DialStyle;

typedef enum { DPS_CIRCLE = 0, DPS_LINE, DPS_SQUARE } DialPointStyle;

typedef enum { DHS_RHOMBUS = 0, DHS_LINE } DialHandStyle;

typedef enum { DNS_ARABIC = 0, DNS_ROAMER } DialNumStyle;

typedef enum { DDT_YES = 1, DDT_NO = 0 } DialDisplayType;

typedef enum { DDT_POSITIVE = 1, DDT_NEGATIVE = 0} DialDeltType;

struct DialPointAttr {
	uint32_t pointSize;
	DialColor pointColor;
	DialPointStyle pointStyle;

public:
	DialPointAttr() {
		pointSize = 1;
		pointColor = KG_COLOR_RED;
		pointStyle = DPS_CIRCLE;
	}

	DialPointAttr operator=(const DialPointAttr &newValue) {
		pointSize = newValue.pointSize;
		pointColor = newValue.pointColor;
		pointStyle = newValue.pointStyle;
		return *this;
	}
	bool operator!=(const DialPointAttr &newValue) {
		if (pointSize != newValue.pointSize || pointColor != newValue.pointColor || pointStyle != newValue.pointStyle) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialPoint {
	DialPointAttr hourPoint;
	DialPointAttr tsnPoint;
	DialPointAttr minutePoint;

public:
	DialPoint() {
		hourPoint.pointSize = 2;
		hourPoint.pointColor = KG_COLOR_RED;
		hourPoint.pointStyle = DPS_CIRCLE;
		tsnPoint.pointSize = 2;
		tsnPoint.pointColor = KG_COLOR_RED;
		tsnPoint.pointStyle = DPS_CIRCLE;
		minutePoint.pointSize = 0;
		minutePoint.pointColor = KG_COLOR_RED;
		minutePoint.pointStyle = DPS_CIRCLE;
	}

	DialPoint operator=(const DialPoint &newValue) {
		hourPoint = newValue.hourPoint;
		tsnPoint = newValue.tsnPoint;
		minutePoint = newValue.minutePoint;
		return *this;
	}
	bool operator!=(const DialPoint &newValue) {
		if (hourPoint != newValue.hourPoint || tsnPoint != newValue.tsnPoint || minutePoint != newValue.minutePoint) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialHandAttr {
	uint32_t handSize;
	DialColor handColor;
	DialHandStyle handStyle;

public:
	DialHandAttr() {
		handSize = 1;
		handColor = KG_COLOR_RED;
		handStyle = DHS_LINE;
	}

	DialHandAttr operator=(const DialHandAttr &newValue) {
		handSize = newValue.handSize;
		handColor = newValue.handColor;
		handStyle = newValue.handStyle;
		return *this;
	}
	bool operator!=(const DialHandAttr &newValue) {
		if (handSize != newValue.handSize || handColor != newValue.handColor || handStyle != newValue.handStyle) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialHand {
	DialHandAttr hourHand;
	DialHandAttr minuteHand;
	DialHandAttr secondHand;

public:
	DialHand() {
		hourHand.handSize = 1;
		hourHand.handColor = KG_COLOR_RED;
		hourHand.handStyle = DHS_LINE;
		minuteHand.handSize = 1;
		minuteHand.handColor = KG_COLOR_RED;
		minuteHand.handStyle = DHS_LINE;
		secondHand.handSize = 1;
		secondHand.handColor = KG_COLOR_RED;
		secondHand.handStyle = DHS_LINE;
	}

	DialHand operator=(const DialHand &newValue) {
		hourHand = newValue.hourHand;
		minuteHand = newValue.minuteHand;
		secondHand = newValue.secondHand;
		return *this;
	}

	bool operator!=(const DialHand &newValue) {
		if (hourHand != newValue.hourHand || minuteHand != newValue.minuteHand || secondHand != newValue.secondHand) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialNumAttr {
	DialNumStyle numStyle;
	DialColor numColor;
	QStr fontName;
	int fontSize;
	DialDisplayType displayType;

public:
	DialNumAttr() {
		numStyle = DNS_ARABIC;
		numColor = KG_COLOR_RED;
		fontName = "";
		fontSize = Font::FONT_SIZE_DEFAULT;
		displayType = DDT_NO;
	}

	DialNumAttr operator=(const DialNumAttr &newValue) {
		numStyle = newValue.numStyle;
		numColor = newValue.numColor;
		fontName = newValue.fontName;
		fontSize = newValue.fontSize;
		displayType = newValue.displayType;
		return *this;
	}

	bool operator!=(const DialNumAttr &newValue) {
		if (numStyle != newValue.numStyle || numColor != newValue.numColor || fontName != newValue.fontName
			|| fontSize != newValue.fontSize || displayType != newValue.displayType) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialNum {
	DialNumAttr hourNum;
	DialNumAttr tsnNum;

public:
	DialNum() {
		hourNum.numColor = KG_COLOR_RED;
		tsnNum.numColor = KG_COLOR_RED;
	}

	DialNum operator=(const DialNum &newValue) {
		hourNum = newValue.hourNum;
		tsnNum = newValue.tsnNum;
		return *this;
	}

	bool operator!=(const DialNum &newValue) {
		if (hourNum != newValue.hourNum || tsnNum != newValue.tsnNum) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialTimeDiff {
	DialDeltType deltType;
	int deltHour;
	int deltMinute;
	int deltSecond;

public:
	DialTimeDiff() {
		deltType = DDT_POSITIVE;
		deltHour = 0;
		deltMinute = 0;
		deltSecond = 0;
	}

	DialTimeDiff operator=(const DialTimeDiff &newValue) {
		deltType = newValue.deltType;
		deltHour = newValue.deltHour;
		deltMinute = newValue.deltMinute;
		deltSecond = newValue.deltSecond;
		return *this;
	}

	bool operator!=(const DialTimeDiff &newValue) {
		if (deltType != newValue.deltType || deltHour != newValue.deltHour || deltMinute != newValue.deltMinute
			|| deltSecond != newValue.deltSecond) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialTime {
	DialPoint dialPoint;
	DialHand dialHand;
	DialNum dialNum;
	DialTimeDiff dialTimeDiff;

public:
	DialTime operator=(const DialTime &newValue) {
		dialPoint = newValue.dialPoint;
		dialHand = newValue.dialHand;
		dialNum = newValue.dialNum;
		dialTimeDiff = newValue.dialTimeDiff;
		return *this;
	}
	bool operator!=(const DialTime &newValue) {
		if (dialPoint != newValue.dialPoint || dialHand != newValue.dialHand || dialNum != newValue.dialNum
			|| dialTimeDiff != newValue.dialTimeDiff) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialFixedTextAttr {
	QStr textFontName;
	int textFontSize;
	DialColor textColor;
	bool bold;
	bool italic;
	bool underLine;
	double xPosRatio;
	double yPosRatio;

public:
	DialFixedTextAttr() {
		textFontName = "";
		textFontSize = Font::FONT_SIZE_DEFAULT;
		textColor = KG_COLOR_RED;
		bold = false;
		italic = false;
		underLine = false;
		xPosRatio = X_RATIO;
		yPosRatio = Y_RATIO;
	}

	DialFixedTextAttr operator=(const DialFixedTextAttr &newValue) {
		textFontName = newValue.textFontName;
		textFontSize = newValue.textFontSize;
		textColor = newValue.textColor;
		bold = newValue.bold;
		italic = newValue.italic;
		underLine = newValue.underLine;
		xPosRatio = newValue.xPosRatio;
		yPosRatio = newValue.yPosRatio;
		return *this;
	}
	bool operator!=(const DialFixedTextAttr &newValue) {
		if (textFontName != newValue.textFontName || textFontSize != newValue.textFontSize
			|| textColor != newValue.textColor || bold != newValue.bold || italic != newValue.italic
			|| underLine != newValue.underLine || xPosRatio != newValue.xPosRatio || yPosRatio != newValue.yPosRatio) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialText {
	bool enabledText;
	QStr text;
	DialFixedTextAttr textAttr;

public:
	DialText() {
		enabledText = false;
		text = "";
	}
	DialText operator=(const DialText &newValue) {
		enabledText = newValue.enabledText;
		text = newValue.text;
		textAttr = newValue.textAttr;
		return *this;
	}
	bool operator!=(const DialText &newValue) {
		if (enabledText != newValue.enabledText || text != newValue.text || textAttr != newValue.textAttr) {
			return true;
		} else {
			return false;
		}
	}
};

struct DialInfo {
	DialStyle dialStyle;
	DialTime dialTime;
	DialText dialText;

public:
	DialInfo() {
		dialStyle = DS_CIRCLE;
	}

	DialInfo operator=(const DialInfo &newValue) {
		dialStyle = newValue.dialStyle;
		dialTime = newValue.dialTime;
		dialText = newValue.dialText;
		return *this;
	}
	bool operator!=(const DialInfo &newValue) {
		if (dialStyle != newValue.dialStyle || dialTime != newValue.dialTime || dialText != newValue.dialText) {
			return true;
		} else {
			return false;
		}
	}
};

class ClockAttr : public RegionAttr
{
	Q_DECLARE_TR_FUNCTIONS(ClockAttr)
public:
	explicit ClockAttr(INodeBox* box, Json* jnode);
	virtual ~ClockAttr();

	DialInfo dialAttrInfo;
	WatchAreaInput input;
	WatchAreaOutput output;

	void toInputAttr(KGCreateMode mode, const char *dir = "");
	void exportProjJson(Json& jnode);
	virtual QString exportCoreJsonArea(Json& jArea) override;
};

#endif // CLOCKATTR_H
