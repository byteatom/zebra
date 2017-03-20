#ifndef KGWATHC_H
#define KGWATHC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "KGMarco.h"

typedef enum _DialType_ {
	DIAL_CIRCLE = 0,
	DIAL_OVAL,
	DIAL_DIAMOND,
	DIAL_SQUARE
} DialType;

typedef enum _HandType_ {
	HAND_DIAMOND = 0,
	HAND_RECTANGLE
} HandType;

typedef enum _ScaleType_ {
	SCALE_CIRCLE = 0,
	SCALE_STRIP
} ScaleType;

typedef struct _DrawDialPara_ {
	KGSize size;
	DialType dialType;
	ScaleType scaleType;
	KGPicType drawPicType;
	KGColor hourColor;
	KGColor quarterColor;
	KGColor crossColor;
	int hourSize;
	int quarterSize;
	int crossSize;
} DrawDialPara;

typedef struct _DrawHandPara_ {
	KGPicType drawPicType;
	KGSize size;
	HandType handType;
	KGColor color;
} DrawHandPara;

typedef struct _WatchAreaInput_ {
	KGCreateMode createMode;
	char dir[PATH_LEN];
	uint8 jetLag[4];			//+1-0, hour, min, sec
	KGCoord coord;
	DrawDialPara dialPara;
	DrawHandPara handPara[3];	//0-hour1-min2-sec
} WatchAreaInput;

typedef struct _WatchHand_
{
	KGCoord coord;
	KGBitmap bitmap;
	char handContent[PATH_LEN];
} WatchHand;

typedef struct _WatchAreaOutput_
{
	uint8 jetLag[4];			//+1-0, hour, min, sec
	WatchHand hourHand[60];
	WatchHand minHand[60];
	WatchHand secHand[60];
	KGBitmap bitmap;
	char dialContent[PATH_LEN];
} WatchAreaOutput;

/*
extern const static int angleTable[60] = {
	90,   84,  78,	72,  66,  60,  54,	48,  42,  36,
	30,   24,  18,	12,   6,   0,  -6, -12, -18, -24,
	-30, -36, -42, -48, -54, -60, -66, -72, -78, -84,
	-90, -96, -102, -108, -114, -120, -126, -132, -138, -144,
	-150, -156, -162, -168, -174, 180, 174, 168, 162, 156,
	150, 144, 138, 132, 126, 120, 114, 108, 102, 96
};
*/

//int __stdcall drawDialPic(const DrawDialPara *drawPara, const char *path, KGPoint *centre);
//int __stdcall drawHandPic(const DrawHandPara *handPara, const int angle, const char *path, KGCoord *coord, KGPoint *centre);

int __stdcall watchAreaWork(const WatchAreaInput *input, WatchAreaOutput *output);
int __stdcall freeWatchArea(WatchAreaOutput *output);

#ifdef __cplusplus
}
#endif

#endif //KGWATHC_H
