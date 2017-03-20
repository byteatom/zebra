#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

#define FIVE_START_POINTS_COUNT 5
//
//
//
void DB_FivePointedStar_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{

    pparams->IsUseCustomStartIco = true;
}
//
/*
void DB_FivePointedStar_GetFiveStarPoints(int vRadius,Sint16 *vxs,Sint16 *vys)
{
    LBM_Point vOrigin;
    LBM_PointF vPoints[FIVE_START_POINTS_COUNT];
    double vAngle;
    LBM_PointF vTemps[FIVE_START_POINTS_COUNT];
    int i;
//
    vOrigin = LBM_MakePoint(0, 0);
    //SetLength(vPoints, 5);
    vAngle = 2.0 * PI / 4 + PI;
    for(i=0; i<=FIVE_START_POINTS_COUNT-1; i++)
    {
        vAngle = vAngle+2.0 * PI / FIVE_START_POINTS_COUNT;
        vPoints[i] = LBM_MakePointF(cos(vAngle) * vRadius + vOrigin.X, sin(vAngle) * vRadius + vOrigin.Y);
    }
    //SetLength(vTemps,Length(vPoints));
    vTemps[0] = vPoints[0];
    vTemps[1] = vPoints[2];
    vTemps[2] = vPoints[4];
    vTemps[3] = vPoints[1];
    vTemps[4] = vPoints[3];

    for(i=0;i<FIVE_START_POINTS_COUNT;i++)
    {
        vxs[i] = vTemps[i].X+vRadius/2;
        vys[i] = vTemps[i].Y+vRadius/2;

    }
}
*/
void DB_FivePointedStar_GetFiveStarPoints(int vRadius,Sint16 *vxs,Sint16 *vys)
{


    const float Ang = 72;
    const float Ang1 = 54;

    double toPI;
    double penR;
    double penInR;
    int i;
    int k;

    toPI = PI/180.0;
    penR = vRadius;
    penInR = penR * sin(18.0*toPI) / sin(126.0*toPI);
    i = 0;
    k = 0;
    //for (int i = 0, k = 0; i < 10; i+=2, k++)
    while(i<10)
    {

        vxs[i] = (penR * cos((Ang1+Ang*k)*toPI))+vRadius;
        vys[i] = (penR * sin((Ang1+Ang*k)*toPI))+vRadius;

        vxs[i+1] = (penInR * cos((Ang*k+(Ang1+36))*toPI))+vRadius;
        vys[i+1] = (penInR * sin((Ang*k+(Ang1+36))*toPI))+vRadius;

        i += 2;
        k++;
    }
    vxs[10] = vxs[0];
    vys[10] = vys[10];

}
//
int DB_FivePointedStar_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
    int index;
    int i;
    int x;
    int y;
    float scaling;

    int rectW,rectH;
    int d;
    LedBitmap *bitmap;

    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;

//
    float *_ds = (float *)malloc(pparams->ShapeCount*sizeof(float));
//
    LBM_Point *_ps = (LBM_Point *)malloc(pparams->ShapeCount*sizeof(LBM_Point));

    int vRadius;

    Sint16 vxs[FIVE_START_POINTS_COUNT*2+1],vys[FIVE_START_POINTS_COUNT*2+1];

    for(i=0; i<=pparams->ShapeCount-1; i++)
    {

        _ds[i] = 0;
        _ps[i] = LBM_MakePoint(0,0);
    }

//
//
    vRadius = 10;
    DB_FivePointedStar_GetFiveStarPoints(vRadius,&vxs[0],&vys[0]);


    //_graphicsPath = TGPGraphicsPath.Create(FillModeWinding);
    //_graphicsPath.AddLines(PGPPointF(vTemps),5);

//
    index = 0;
    while((pparams->CurrentLoopIndex < pparams->LoopCount))
    {
        for(i=0; i<=pparams->ShapeCount-1; i++)
        {
            if( (_ps[i].Y > input->coord.height)  ||  (_ps[i].Y == 0))
            {
                pparams->CurrentLoopIndex++;
                _ps[i] = LBM_MakePoint(DB_Base_GetRandom(input->coord.width), DB_Base_GetRandom(input->coord.height));
//
                _ds[i] = DB_Base_GetRandom(5) + 5;
            }
        }
        for(i=0; i<=pparams->ShapeCount-1; i++)
        {
            _ps[i].Y = _ps[i].Y+3;
            _ds[i] = _ds[i]+0.3;
            x = _ps[i].X;
            y = _ps[i].Y;
//
            scaling = _ds[i] / 10;
            //graphicPath = _graphicsPath.Clone();
//
            //matrix = TGPMatrix.Create(scaling, 0, 0, scaling, 0, 0);
            //graphicPath.Transform(matrix);
            //graphicPath.GetBounds(rectangleF);
            LBM_GetZoomSize(vRadius,vRadius,scaling,&rectW,&rectH);
            d = ceil(rectW);
            if (d <= 0)
            {
                d = 0;
            }
            if( !MapIntInt_ContainsKey(&(pparams->BitmapInfoDictionaries),d))
            {
                if (d > 0)
                {
                    //bitmap = LBM_CreateLedBitmap(d, d,input->drawPicType);
                    bitmap = LBM_CreateLedBitmap(d, d,input->drawPicType);


                    //sb = TGPSolidBrush.Create(TLedColor.TLedColor(clRed).AColor);
                    //graphics.FillPath(sb, graphicPath);
                    DB_FivePointedStar_GetFiveStarPoints(d/2,&vxs[0],&vys[0]);
                    filledPolygonColor(bitmap, &vxs[0], &vys[0], FIVE_START_POINTS_COUNT*2, LBM_KGColorToSdlColor(KG_COLOR_RED));
                    polygonColor(bitmap, &vxs[0], &vys[0], FIVE_START_POINTS_COUNT*2, LBM_KGColorToSdlColor(KG_COLOR_RED));

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

                //_elementInitialInfo = ElementInitialInfo.Create();
                //_elementInitialInfo.LastX = x;
                //_elementInitialInfo.StartX = x;
                //_elementInitialInfo.LastY = y;
                //_elementInitialInfo.StartY = y;
                //_elementInitialInfo.StartIco = BitmapInfoDictionaries[d];
                //ElementInitialInfos[i] = _elementInitialInfo;
            }
            if (d > 0)
            {
                if (index > 0)
                {
                    //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
                    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                    _locusparadcb->nextPic = 1;
                    _locusparadcb->offsetX = (x - _elementInitialInfo->lastX);
                    _locusparadcb->offsetY = (y - _elementInitialInfo->lastY);
                    _locusparadcb->pictureNumber = MapIntInt_ValueAt(&(pparams->BitmapInfoDictionaries),d);
                    _locusparadcb->recycleTotal = 0;
                    _elementInitialInfo->lastX = x;
                    _elementInitialInfo->lastY = y;

                    //_locusparadcb.BNext = 1;
                    //_locusparadcb.XOffset = (x - _elementInitialInfo.LastX);
                    //_locusparadcb.YOffset = (y - _elementInitialInfo.LastY);
                    //_locusparadcb.PNum = BitmapInfoDictionaries[d];

                    //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb);

                    //_elementInitialInfo.LastX = x;
                    //_elementInitialInfo.LastY = y;
                }
            }
            else
            {
                if (index > 0)
                {
//
                    //PNum := -1 ));
                    //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(-1);
                    //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb);
                    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                    _locusparadcb->pictureNumber = -1;
                }
            }
        }
        index++;
    }
    free(_ds);
    free(_ps);
    return result;
}
int DB_FivePointedStar(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_FivePointedStar_Create(input, output, pparams);
    return DB_FivePointedStar_MakeBinaryDetail(input, output, pparams);
}
