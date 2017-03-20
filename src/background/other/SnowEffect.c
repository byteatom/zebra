#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

//
//
//
void DB_Snow_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{

    pparams->IsUseCustomStartIco = true;
}
//
void DB_Snow_DrawBaseStar(LedBitmap *bitmap, Sint16 centerX, Sint16 centerY, KGColor color)
{
    int w;
    int h;
    int h2;
    int w2;
    int i;
    LBM_PointF points[4];

    Sint16 vxs[4],vys[4];

//

    w = 2;
    h = 10;
    h2 = 8;
    w2 = 2;

    points[0] = LBM_MakePointF(0, h);
    points[1] = LBM_MakePointF(w, 0);
    points[2] = LBM_MakePointF(0, -h);
    points[3] = LBM_MakePointF(-w, 0);
    for(i=0; i<4; i++)
    {
        vxs[i] = ceil(points[i].X)+centerX;
        vys[i] = ceil(points[i].Y)+centerY;
    }
    filledPolygonColor(bitmap, &vxs[0], &vys[0], 4, LBM_KGColorToSdlColor(color));

    points[0] = LBM_MakePointF(0, w);
    points[1] = LBM_MakePointF(h, 0);
    points[2] = LBM_MakePointF(0, -w);
    points[3] = LBM_MakePointF(-h, 0);
    for(i=0; i<4; i++)
    {
        vxs[i] = ceil(points[i].X)+centerX;
        vys[i] = ceil(points[i].Y)+centerY;
    }
    filledPolygonColor(bitmap, &vxs[0], &vys[0], 4, LBM_KGColorToSdlColor(color));

    points[0] = LBM_MakePointF(-h2 * cos(45 * PI / 180), h2 * sin(45 * PI / 180));
    points[1] = LBM_MakePointF(w2 * cos(45 * PI / 180), w2 * sin(45 * PI / 180));
    points[2] = LBM_MakePointF(h2 * cos(45 * PI / 180), -h2 * sin(45 * PI / 180));
    points[3] = LBM_MakePointF(-w2 * cos(45 * PI / 180), -w2 * sin(45 * PI / 180));
    for(i=0; i<4; i++)
    {
        vxs[i] = ceil(points[i].X)+centerX;
        vys[i] = ceil(points[i].Y)+centerY;
    }
    filledPolygonColor(bitmap, &vxs[0], &vys[0], 4, LBM_KGColorToSdlColor(color));

    points[0] = LBM_MakePointF(-w2 * cos(45 * PI / 180), w2 * sin(45 * PI / 180));
    points[1] = LBM_MakePointF(h2 * cos(45 * PI / 180), h2 * sin(45 * PI / 180));
    points[2] = LBM_MakePointF(w2 * cos(45 * PI / 180), -w2 * sin(45 * PI / 180));
    points[3] = LBM_MakePointF(-h2 * cos(45 * PI / 180), -h2 * sin(45 * PI / 180));
    for(i=0; i<4; i++)
    {
        vxs[i] = ceil(points[i].X)+centerX;
        vys[i] = ceil(points[i].Y)+centerY;
    }
    filledPolygonColor(bitmap, &vxs[0], &vys[0], 4, LBM_KGColorToSdlColor(color));
}
//
int DB_Snow_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;

    // <summary>
//
    //</summary>
    const int _dx[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    // <summary>
//
    // </summary>
    const _dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1 };


    int index;
    int i;
    char t[STR_BUF_COUNT];
    int tm;
    int x;
    int y;
    float scaling;
    int rectW,rectH;
    int d;
    LedBitmap *bitmap,*bitmapBase;

    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;

//
    float *_ds = (float *)malloc(pparams->ShapeCount*sizeof(float));
//
    LBM_Point *_ps = (LBM_Point *)malloc(pparams->ShapeCount*sizeof(LBM_Point));
//
//
    char **_directions = (char **)malloc(pparams->ShapeCount*sizeof(char *));//[STR_BUF_COUNT];

    int temp = 0;
    int vRadius = 10;

    for(i=0; i<pparams->ShapeCount; i++)
    {
        _ds[i] = 0;
        _ps[i] = LBM_MakePoint(0,0);
        _directions[i] = (char *)malloc(STR_BUF_COUNT);
        memset(_directions[i],0,STR_BUF_COUNT);
    }

    bitmapBase = LBM_CreateLedBitmap(vRadius,vRadius,input->drawPicType);
    DB_Snow_DrawBaseStar(bitmapBase, vRadius/2, vRadius/2, KG_COLOR_RED);
//
    index = 0;
    while((pparams->CurrentLoopIndex < pparams->LoopCount))
    {
        for(i=0; i<=pparams->ShapeCount-1; i++)
        {
            if (_ds[i] <= 0)
            {
                pparams->CurrentLoopIndex++;
                _ds[i] = DB_Base_GetRandom(15);
                _ps[i] = LBM_MakePoint((DB_Base_GetRandom((input->coord.width * 0.8)) + input->coord.width * 0.2), DB_Base_GetRandom(input->coord.height));
            }
        }
        for(i=0; i<=pparams->ShapeCount-1; i++)
        {
            if (strlen(_directions[i])==0)
            {
                //t = IntToStr(DB_Base_GetRandom(8));
                sprintf(t,"%d",DB_Base_GetRandom(8));
                StringPadRight(t,DB_Base_GetRandom(10) + 5, t[0],_directions[i]);
            }
            //tm = StrToInt(_directions[i][1]);
            tm = _directions[i][0]-'0';
            //_directions[i] := _directions[i].Remove(0, 1);
            String_Delete(_directions[i],1,1);
            _ps[i].X = _ps[i].X+_dx[tm] * 4;
            _ps[i].Y = _ps[i].Y+_dy[tm] * 4;
            _ds[i] = _ds[i]-0.5;
            x = _ps[i].X;
            y = _ps[i].Y;
            scaling = _ds[i] / vRadius;
            //graphicPath = _graphicsPath.Clone();
            if(temp >= 360)
            {
                temp = 0;
            }
            //matrix = TGPMatrix.Create(scaling, 0, 0, scaling, 0, 0);
            //matrix.Rotate(temp);

            //graphicPath.Transform(matrix);
            //graphicPath.GetBounds(rectangleF);
            LBM_GetRotoZoomSize(vRadius,vRadius,temp,scaling,&rectW,&rectH);
            d = ceil(rectW);
            temp++;
            if (d <= 0)
            {
                d = 0;
            }
            if( !MapIntInt_ContainsKey(&(pparams->BitmapInfoDictionaries),d))
            {
                if (d > 0)
                {
                    bitmap = rotozoomSurface(bitmapBase,temp-1,scaling,0);

                    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);

                    LBM_FreeLedBitmap(bitmap);
                }
                else
                {
                    result = DB_Base_AddPlaceHolderBitmap(&(pparams->Bitmaps),input->dir,input->drawPicType,input->createMode);
                }
                MapIntInt_Add(&(pparams->BitmapInfoDictionaries),d, DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - 1);
            }
            _elementInitialInfo = DB_Base_ShineElementTableAt(&(pparams->Elements),i);
            if (_elementInitialInfo == NULL)
            {
//
                _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
                _elementInitialInfo->startX = x;
                _elementInitialInfo->startY = y;
                _elementInitialInfo->lastX = x;
                _elementInitialInfo->lastY = y;
                _elementInitialInfo->startPictureNumber = MapIntInt_ValueAt(&(pparams->BitmapInfoDictionaries),d);
                _elementInitialInfo->locusTable.isnull = true;
                _elementInitialInfo->locusTable.next = NULL;
            }
            //int width = (int) Ceiling(rectangleF.Width);
            //int height = (int) Ceiling(rectangleF.Height);
            if(d > 0)
            {
//
                if (index > 0)
                {
                    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                    _locusparadcb->nextPic = 1;
                    _locusparadcb->offsetX = (x - _elementInitialInfo->lastX);
                    _locusparadcb->offsetY = (y - _elementInitialInfo->lastY);
                    _locusparadcb->pictureNumber = MapIntInt_ValueAt(&(pparams->BitmapInfoDictionaries),d);
                    _locusparadcb->recycleTotal = 0;
                    _elementInitialInfo->lastX = x;
                    _elementInitialInfo->lastY = y;
                }
            }
            else
            {
                if (index > 0)
                {
//
                    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                    _locusparadcb->pictureNumber = -1;
                }
            }
        }
        index++;
    }
    LBM_FreeLedBitmap(bitmapBase);
    free(_ds);
    free(_ps);
    for(i=0; i<pparams->ShapeCount; i++)
    {
        free(_directions[i]);
    }
    free(_directions);
    return result;
}
int DB_Snow(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_Snow_Create(input, output, pparams);
    return DB_Snow_MakeBinaryDetail(input, output, pparams);
}
