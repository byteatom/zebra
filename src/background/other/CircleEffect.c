#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"
#include "TileEffectBase.h"

void DB_Circle_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams, TileBaseParams *tileParams)
{
    DB_Tile_Create(input, output, pparams, tileParams);
}

//
//
//
void DB_Circle_GetGraphicsPath(TileBaseParams *tileParams, LedBitmap *bitmap, int index, LBM_RectF *prectangleF,KGColor color)
{

  //graphicsPath.AddEllipse((tileParams->MaxD - index) / 2.0, (tileParams->MaxD - index) / 2.0, index,
  //  index);
    circleColor(bitmap,tileParams->MaxD/2,tileParams->MaxD/2,index/2,LBM_KGColorToSdlColor(color));
    prectangleF->X = (tileParams->MaxD - index) / 2.0;
    prectangleF->Y = (tileParams->MaxD - index) / 2.0;
    prectangleF->Width = index;
    prectangleF->Height = index;
  // Result := TDynamicBackgroundElement.GetGraphicsPath(index,MaxD,0);
}

//
int DB_Circle_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams, TileBaseParams *tileParams)
{
    return DB_Tile_MakeBinaryDetail(input, output, pparams, tileParams,NULL, DB_Circle_GetGraphicsPath);
}

int DB_Circle(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    TileBaseParams tileParams;
    DB_Circle_Create(input, output, pparams, &tileParams);
    return DB_Circle_MakeBinaryDetail(input, output, pparams, &tileParams);
}
