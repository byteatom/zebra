#ifndef ISCREEN_H
#define ISCREEN_H

#include <QPoint>
#include <QSize>
#include <QTime>
#include <deque>
#include "QStr.h"
#include "Attr.h"
#include "IProtocol.h"
#include "Message.h"

class IScreen
{
public:
	enum BaseColor {ONE = 0, TWO, THREE};
	enum CardSeries {CLASSIAL = 0, COLORFUL};
	enum WarpMode {UP = 0, DOWN, STRAIGHT, SCANDOT};
	enum DecodeMode {HIGH_138 = 0, LOW_138, HIGH_IO, LOW_IO};
	enum GrayScale {NONE = 0, GRAY_4, GRAY_8, GRAY_16, GRAY_32, GRAY_256};

	struct TopCfg {
		QStr outFilePath;
		QStr privateModel;
		QStr dataEndian;
		int colorThreshold;
	};

	struct Module {
		QStr model;
		bool change;
		Attr<int> baseColor;	//enum BaseColor
		int moduleLines;
		int scanLines;
		int warpTimes;
		int warpBits;
		int warpMode;	//enum WarpMode
		int decodeMode;	//enum DecodeMode
		bool dataPolarity;
		bool strPolarity;
		bool oePolarity;
		bool mirror;
		bool split;
		bool warpTurn;
		bool scanDot;
		int moduleWidth;
		int moduleHeight;
		int emptyDot;
		struct {
			int x;
			int y;
			bool empty;
		} dotMap[2048];
		int lineMap[32];
		int dotIndex;
		int emptyDotIndex;
		int color1;
		int color2;
		int color3;
		int color4;
		bool expandRGB;
		bool complete;
	};

	struct ScreenPara {
		QStr cardModel;
		QStr ledModel;
		Attr<QSize> size;
		bool dataPolarity;
		bool oePolarity;
		bool strPolarity;
		int cardSeries;
		int baseColor;	//enum BaseColor
		int grayScale;	//enum GrayScale
		int threshold;
		int lineVerify;
		int frequency;
		int lineBlank;
		double gama;
		int configIndex;
		bool autoMode;
		bool seniorCfgHide;
	};

	struct OnOff {
		bool timerSwitch;
		QTime openTime;
		QTime closeTime;
	};

	struct SyncTime {
		bool manualSel;
		bool systemSel;
		bool relativeTime;
		QTime time;
		QDate date;
	};

	struct Bright {
		bool manualSel;
		bool timingSel;
		bool autoSel;
		int manualBright;
		bool timingCheck[4];
		QTime startTime[4];
		QTime endTime[4];
		int timingBright[4];
		int defaultBright;
	};

	virtual qreal zoomFactor() = 0;
	virtual Module& module() = 0;
	virtual ScreenPara& screenPara() = 0;
	virtual OnOff& onOff() = 0;
	virtual SyncTime& syncTime() = 0;
	virtual Bright& bright() = 0;

	virtual QString buildFiles(const QString& dir, bool allEndian) = 0;
	virtual IProtocol* protocol() = 0;

protected:
	virtual ~IScreen() = default;
};

#endif // ISCREEN_H

