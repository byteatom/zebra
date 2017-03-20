#ifndef DYNAMICBASE_H_INCLUDED
#define DYNAMICBASE_H_INCLUDED

#include "Common.h"
#include "LedBitmap.h"
#include "MapIntInt.h"

typedef struct _ShineLocusTable_
{
    bool isnull;
	int pictureNumber;
	int offsetX;
	int offsetY;
	int nextPic;
	int recycleTotal;
	struct _ShineLocusTable_ *next;
} ShineLocusTable;

typedef struct _ShineElementTable_
{
    bool isnull;
	int locusTotal;
	int startX;
	int startY;
	int lastX;
	int lastY;
	int startPictureNumber;
	ShineLocusTable locusTable;
	struct _ShineElementTable_ *next;
} ShineElementTable;

typedef struct _ShinePictureTable_
{
    int index;
    bool isnull;
	int width;
	int height;
	KGBitmap bitmap;
	char pictureContent[PATH_LEN];
	struct _ShinePictureTable_ *next;
} ShinePictureTable;



int DB_Base_GetRandom(int max);
ShineElementTable *DB_Base_ShineElementTableAt(ShineElementTable *elements,int index);
ShineElementTable *DB_Base_ShineElementTableAdd(ShineElementTable *elements);
ShineLocusTable *DB_Base_ShineLocusTableAdd(ShineLocusTable *locuss);
ShinePictureTable *DB_Base_ShinePictureTableAdd(ShinePictureTable *bitmaps);
int DB_Base_ShinePictureTableCount(ShinePictureTable *bitmaps);
int DB_Base_BoxingBitmap(ShinePictureTable *bitmaps,const char *dir,LedBitmap *bitmap,KGCreateMode createMode);
void DB_Base_FillCustumColor(LedBitmap *bmpDest, SDL_Rect rcDest,KGColor color, LedBitmap *bmpColor, bool bTile);

#endif // DYNAMICBASE_H_INCLUDED
