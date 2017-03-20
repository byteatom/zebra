#ifndef KGBACKGROUND_H
#define KGBACKGROUND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "KGMarco.h"

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

typedef enum _KGBackgroundStyle
{
	BGS_Cirque = 0,
	BGS_FivePointedStar,
	BGS_Babysbreath,
	BGS_Snow,
	BGS_Arrow,
	BGS_SideswaySlipLine,
	BGS_FaceToFaceSlipLine,
	BGS_Diamond,
	BGS_Block,
	BGS_Circle,
	BGS_CrewelScan,
	BGS_SingleScan
}KGBackgroundStyle;

typedef struct _BackgroundInput_ {
	KGCreateMode createMode;
	KGPicType drawPicType;
	char dir[PATH_LEN];
	KGCoord coord;
	int style;
	int speed;
} BackgroundInput;

typedef struct _BackgroundOutput_
{
	int frameTotal;
	int elementTotal;
	int pictureTotal;
	ShineElement *elementPtr;
	ShinePicture *picturePtr;
} BackgroundOutput;

int __stdcall backgroundWork(const BackgroundInput *input, BackgroundOutput *output);
int __stdcall freeBackground(BackgroundOutput *output);



#ifdef __cplusplus
}
#endif

#endif //KGBACKGROUND_H
