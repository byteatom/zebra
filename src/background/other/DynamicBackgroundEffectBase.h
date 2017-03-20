#ifndef DYNAMICBACKGROUNDEFFECTBASE_H_INCLUDED
#define DYNAMICBACKGROUNDEFFECTBASE_H_INCLUDED

#include "DynamicBase.h"
#include "KGBackground.h"

#define STR_BUF_COUNT 10240


typedef struct _BgElementBaseParams
{
    int GroupCount;
    int ShapeCount;
    int CurrentLoopIndex;
    int LoopCount;
    MapIntInt BitmapInfoDictionaries;
    bool IsUseCustomStartIco;
    bool IsOverrideFrameCount;
    ShineElementTable Elements;
    ShinePictureTable Bitmaps;
}BgElementBaseParams;




int DB_Base_BoxingBitmap(ShinePictureTable *bitmaps,const char *dir,LedBitmap *bitmap,KGCreateMode createMode);
int DB_Base_AddPlaceHolderBitmap(ShinePictureTable *bitmaps,const char *dir,KGPicType pictype,KGCreateMode createMode);

#endif // DYNAMICBACKGROUNDEFFECTBASE_H_INCLUDED
