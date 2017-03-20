#ifndef DYNAMICTEXTEFFECTBASE_H_INCLUDED
#define DYNAMICTEXTEFFECTBASE_H_INCLUDED

#include "DynamicBase.h"
#include "KGShineFont.h"

typedef enum _RegionState
{
    //
    Normal = 0,
    //
    AllLight,
    //
    AllDark
}RegionState;

typedef struct _DtElementBaseParams
{
    bool AllText;
    RegionState State;
    int CurrentCharIndex;
    ShineElementTable Elements;
    ShinePictureTable Bitmaps;
    int TextCount;
    int TextWidth;
    int TextHeight;
    LedBitmap *ColorBitmap;

}DtElementBaseParams;

typedef enum _RainbowDirection
{
    rdUp=0,
    rdDown,
    rdLeft,
    rdRight
} RainbowDirection;


#endif // DYNAMICTEXTEFFECTBASE_H_INCLUDED
