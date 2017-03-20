#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"
#include "TileEffectBase.h"

//
// <param name="programInfo"></param>
// <param name="bounds"></param>
void DB_Block_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams, TileBaseParams *tileParams)
{
    DB_Tile_Create(input, output, pparams, tileParams);
}
//
//
//
void DB_Block_GetGraphicsPath(TileBaseParams *tileParams, LedBitmap *bitmap, int index, LBM_RectF *prectangleF,KGColor color)
{

    LBM_PointF points[4];
    Sint16 vxs[4],vys[4];
    int i;





    points[0] = LBM_MakePointF((tileParams->MaxD - index) / 2, (tileParams->MaxD - index) / 2);
    points[1] = LBM_MakePointF((tileParams->MaxD+ index) / 2 , (tileParams->MaxD - index) / 2);
    points[2] = LBM_MakePointF((tileParams->MaxD+ index) / 2 , (tileParams->MaxD+ index) / 2 );
    points[3] = LBM_MakePointF((tileParams->MaxD - index) / 2, (tileParams->MaxD+ index) / 2 );

    for(i=0; i<4; i++)
    {
        vxs[i] = ceil(points[i].X);
        vys[i] = ceil(points[i].Y);
    }
    polygonColor(bitmap, &vxs[0], &vys[0], 4, LBM_KGColorToSdlColor(color));

    prectangleF->X = (tileParams->MaxD - index) / 2.0;
    prectangleF->Y = (tileParams->MaxD - index) / 2.0;
    prectangleF->Width = index;
    prectangleF->Height = index;
}

//
int DB_Block_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams, TileBaseParams *tileParams)
{
    return DB_Tile_MakeBinaryDetail(input, output, pparams, tileParams,NULL, DB_Block_GetGraphicsPath);
}

int DB_Block(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    TileBaseParams tileParams;
    DB_Block_Create(input, output, pparams, &tileParams);
    return DB_Block_MakeBinaryDetail(input, output, pparams, &tileParams);
}
