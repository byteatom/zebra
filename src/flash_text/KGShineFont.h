#ifndef KGSHINEFONT_H
#define KGSHINEFONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "KGMarco.h"

#define MAX_TEXT_LEN	300
#define MAX_COLOR_COUNT	255

#ifndef USE_SHINE_STRUCT
#define USE_SHINE_STRUCT
typedef struct _ShineLocus_
{
	int pictureNumber;
	int offsetX;
	int offsetY;
	int nextPic;
	int recycleTotal;
} ShineLocus;

typedef struct _ShineElement_
{
	int locusTotal;
	int startX;
	int startY;
	int startPictureNumber;
	ShineLocus *locusPtr;
} ShineElement;

typedef struct _ShinePicture_
{
	int width;
	int height;
	KGBitmap bitmap;
	char pictureContent[PATH_LEN];
} ShinePicture;
#endif //USE_SHINE_STRUCT

typedef enum _FontColorMode_
{
	FONT_COLOR_MODE_COLOR = 0,
	FONT_COLOR_MODE_RANDOM,
	FONT_COLOR_MODE_FILE,
} FontColorMode;

typedef enum _ShineFontType_
{
	SFT_HeartBeat = 0,
	SFT_Waltz,
	SFT_Excursion,
	SFT_RadioWave,
	SFT_FlashBulb,
	SFT_Spring,
	SFT_RollOver,
	SFT_Domino,
	SFT_ExcursionReverse,
	SFT_Swirling,
	SFT_NeonLight,
	SFT_RandomColor,
	SFT_RainbowUp,
	SFT_RainbowDown,
	SFT_RainbowLeft,
	SFT_RainbowRight
} ShineFontType;

typedef struct _ShineFontAreaInput_ {
	KGCreateMode createMode;
	KGPicType drawPicType;
	char dir[PATH_LEN];
	KGCoord coord;
	int style;
	int speed;
	unsigned char bold;
	unsigned char italic;
	unsigned char underline;
	FontColorMode colorMode;
	int colorCount;
	KGColor colors[MAX_COLOR_COUNT];
	int fontSize;
	char text[MAX_TEXT_LEN];    //UTF8
	char fontFile[PATH_LEN];
	char colorFile[PATH_LEN];
	unsigned char isColorTile;
} ShineFontAreaInput;

typedef struct _ShineFontAreaOutput_
{
	int frameTotal;
	int elementTotal;
	int pictureTotal;
	ShineElement *elementPtr;
	ShinePicture *picturePtr;
} ShineFontAreaOutput;

int __stdcall shineFontAreaWork(const ShineFontAreaInput *input, ShineFontAreaOutput *output);
int __stdcall freeShineFontArea(ShineFontAreaOutput *output);

#ifdef __cplusplus
}
#endif

#endif //KGSHINEFONT_H
