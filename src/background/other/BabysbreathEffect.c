#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"


//
//
//
void DB_Babysbreath_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{



    pparams->GroupCount = pparams->ShapeCount;
    pparams->LoopCount = pparams->LoopCount+(pparams->GroupCount * pparams->ShapeCount);


    pparams->IsUseCustomStartIco = true;
}
//
void DB_Babysbreath_DrawBaseStar(LedBitmap *bitmap, Sint16 centerX, Sint16 centerY, KGColor color)
{
    int i;
    int w;
    int h;
    int h2;
    int w2;
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
    //_graphicsPath.AddPolygon(PGPPointF(points),Length(points));
    points[0] = LBM_MakePointF(0, w);
    points[1] = LBM_MakePointF(h, 0);
    points[2] = LBM_MakePointF(0, -w);
    points[3] = LBM_MakePointF(-h, 0);
    //_graphicsPath.AddPolygon(PGPPointF(points),Length(points));
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
    //_graphicsPath.AddPolygon(PGPPointF(points),Length(points));
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
    //_graphicsPath.AddPolygon(PGPPointF(points),Length(points));
    for(i=0; i<4; i++)
    {
        vxs[i] = ceil(points[i].X)+centerX;
        vys[i] = ceil(points[i].Y)+centerY;
    }
    filledPolygonColor(bitmap, &vxs[0], &vys[0], 4, LBM_KGColorToSdlColor(color));
}
//
int DB_Babysbreath_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
//
    KGColor _dcolors[3] = {KG_COLOR_RED,
                           KG_COLOR_GREEN,KG_COLOR_YELLOW
                          };
//
    int _dx[8] = {0,1,1,1,0,-1,-1,-1};
//
    int _dy[8] = {-1,-1,0,1,1,1,0,-1};
    int len = pparams->ShapeCount * pparams->GroupCount;

    int index;
    int g;
    int vx;
    int vy;
    int i;
    int m;
    ShineElementTable *tempElementInitialInfo;
    char t[STR_BUF_COUNT];
    int tm;
    int x;
    int y;
    float scaling;
    int rectW,rectH;
    //TGPGraphicsPath graphicsPath;
    //TGPMatrix matrix;
    //TGPRectF rectangleF;
    int d;
    LedBitmap *bitmap,*bitmapBase;

    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;

//
    float *_ds = (float *)malloc(len*sizeof(float));

//
    float *_da = (float *)malloc(len*sizeof(float));

//
    KGColor *_dc = (KGColor *)malloc(len*sizeof(KGColor));

//
    LBM_Point *_ps = (LBM_Point *)malloc(len*sizeof(LBM_Point));

//
//
    char **_directions = (char **)malloc(len*sizeof(char *));//[len][STR_BUF_COUNT];

//
    //_dcolors: TListColor;//Color[](clRed: ;clGreen: ;clYellow: ): new;
//
    //_dx: TIntegerDynArray;//int[](0: ;1: ;1: ;1: ;0: ;-1: ;-1: ;-1: ): new;
//
    //_dy: TIntegerDynArray;//int[](-1: ;-1: ;0: ;1: ;1: ;1: ;0: ;-1: ): new;
//
    int *HaveExplosedNums = (int *)malloc(pparams->GroupCount*sizeof(int));//[pparams->GroupCount];

//
    int *ExplosionSpeeds = (int *)malloc(pparams->GroupCount*sizeof(int));//[pparams->GroupCount];

//
    bool *IsExploseds = (bool *)malloc(len*sizeof(bool));


    int vRadius = 10;

    for(i=0; i<=pparams->GroupCount-1; i++)
    {
        HaveExplosedNums[i] = pparams->ShapeCount;
        ExplosionSpeeds[i] = 0;
    }
    for(i=0; i<len; i++)
    {
        _ds[i] = 0;
        _da[i] = 0;
        _dc[i] = KG_COLOR_RED;
        _ps[i] = LBM_MakePoint(0,0);
        _directions[i] = (char *)malloc(STR_BUF_COUNT*sizeof(char));

        memset(_directions[i],0,STR_BUF_COUNT);
    }

    bitmapBase = LBM_CreateLedBitmap(vRadius,vRadius,input->drawPicType);
    DB_Babysbreath_DrawBaseStar(bitmapBase, vRadius/2, vRadius/2, KG_COLOR_RED);
//
    index = 0;
    while((pparams->CurrentLoopIndex < pparams->LoopCount))
    {
        for(g=0; g<=pparams->GroupCount-1; g++)
        {
            if (HaveExplosedNums[g] >= pparams->ShapeCount)
            {
                ExplosionSpeeds[g] = DB_Base_GetRandom(5) + 1;
                vx = DB_Base_GetRandom((input->coord.width * 0.6)) + (input->coord.width * 0.2);
                vy = DB_Base_GetRandom((input->coord.height * 0.6)) + (input->coord.height * 0.2);
                for(i=0; i<=pparams->ShapeCount-1; i++)
                {
                    pparams->CurrentLoopIndex++;
                    m = i + g*pparams->ShapeCount;
                    _ds[m] = DB_Base_GetRandom(20);
                    _da[m] = DB_Base_GetRandom(360);
                    _dc[m] = _dcolors[DB_Base_GetRandom(3)];
                    _ps[m] = LBM_MakePoint(vx, vy);
                    IsExploseds[m] = false;
                    memset(_directions[m],0,sizeof(_directions[m]));
                }
                HaveExplosedNums[g] = 0;
            }
        }
        for(g=0; g<=pparams->GroupCount-1; g++)
        {
            for(i=0; i<=pparams->ShapeCount-1; i++)
            {
                m = i + g * pparams->ShapeCount;
                if (IsExploseds[m])
                {
                    if (index > 0)
                    {
//
                        tempElementInitialInfo = DB_Base_ShineElementTableAt(&(pparams->Elements),m);
//
                        //tempElementInitialInfo.Locusparadcbs.Add(LOCUSPARADCB.LOCUSPARADCB(-1));
                        _locusparadcb = DB_Base_ShineLocusTableAdd(&(tempElementInitialInfo->locusTable));
                        _locusparadcb->pictureNumber = -1;
                    }
                    continue;
                }
                //if (string.IsNullOrEmpty(_directions[m])) then
                if (strlen(_directions[m])==0)
                {
                    //t = IntToStr(DB_Base_GetRandom(8));
                    sprintf(t,"%d",DB_Base_GetRandom(8));
                    //_directions[m] := t.PadRight(Max(input->coord.width, input->coord.height), t.ToCharArray()[0]);
                    StringPadRight(t,max(input->coord.width, input->coord.height), t[0],_directions[m]);
                }
                //tm = StrToInt(_directions[m][1]);
                tm = _directions[m][0]-'0';
                //_directions[m] := _directions[m].Remove(0, 1);
                String_Delete(_directions[m],1,1);
                _ps[m].X = _ps[m].X+_dx[tm] * DB_Base_GetRandom(3);
                _ps[m].Y = _ps[m].Y+_dy[tm] * DB_Base_GetRandom(3);
                _ds[m] = _ds[m]-(2.0 / ExplosionSpeeds[g]);
                if (_ds[m] <= 0)
                {
                    HaveExplosedNums[g]++;
                    IsExploseds[m] = true;
                }
                x = _ps[m].X;
                y = _ps[m].Y;
//
                scaling = _ds[m] / vRadius;
                //graphicsPath = _graphicsPath.Clone();
                //matrix = TGPMatrix.Create(scaling, 0, 0, scaling, 0, 0);
                //matrix.Rotate(_da[m]);
                //graphicsPath.Transform(matrix);
//
                //graphicsPath.GetBounds(rectangleF);
                //int width = (int) Ceiling(rectangleF.Width);
                //int height = (int) Ceiling(rectangleF.Height);

                LBM_GetRotoZoomSize(vRadius,vRadius,_da[m],scaling,&rectW,&rectH);
                d = ceil(rectW);
                if (d <= 0)
                {
                    d = 0;
                }
                if( !MapIntInt_ContainsKey(&(pparams->BitmapInfoDictionaries),d))
                {
                    //Console.WriteLine(d);
                    if (d > 0)
                    {
                        //bitmap = LBM_CreateLedBitmap(d, d,input->drawPicType);
                        bitmap = rotozoomSurface(bitmapBase,_da[m],scaling,0);

                        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);

                        LBM_FreeLedBitmap(bitmap);
                    }
                    else
                    {
                        result = DB_Base_AddPlaceHolderBitmap(&(pparams->Bitmaps),input->dir,input->drawPicType,input->createMode);
                    }
                    MapIntInt_Add(&(pparams->BitmapInfoDictionaries),d, DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - 1);
                }
                _elementInitialInfo = DB_Base_ShineElementTableAt(&(pparams->Elements),m);
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
                    //_elementInitialInfo.LastX = Int16(x);
                    //_elementInitialInfo.StartX = Int16(x);
                    //_elementInitialInfo.LastY = Int16(y);
                    //_elementInitialInfo.StartY = Int16(y);
                    //_elementInitialInfo.StartIco = Int16(BitmapInfoDictionaries[d]);
                    //ElementInitialInfos[m] = _elementInitialInfo;
                }
                if(d > 0)
                {
//
                    if (index > 0)
                    {
                        //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(-1);
                        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                        _locusparadcb->nextPic = 1;
                        _locusparadcb->offsetX = (x - _elementInitialInfo->lastX);
                        _locusparadcb->offsetY = (y - _elementInitialInfo->lastY);
                        _locusparadcb->pictureNumber = MapIntInt_ValueAt(&(pparams->BitmapInfoDictionaries),d);
                        _locusparadcb->recycleTotal = 0;
                        _elementInitialInfo->lastX = x;
                        _elementInitialInfo->lastY = y;

                        //_locusparadcb.BNext = 1;
                        //_locusparadcb.XOffset = ShortInt(x - _elementInitialInfo.LastX);
                        //_locusparadcb.YOffset = ShortInt(y - _elementInitialInfo.LastY);
                        //_locusparadcb.PNum = Int16(BitmapInfoDictionaries[d]);
                        //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb);
                        //  _elementInitialInfo.Locusparadcbs.Add(LOCUSPARADCB.LOCUSPARADCB(1, (x - _elementInitialInfo.LastX),
                        //    (y - _elementInitialInfo.LastY), BitmapInfoDictionaries[d]));

                        //_elementInitialInfo.LastX = Int16(x);
                        //_elementInitialInfo.LastY = Int16(y);
                    }
                }
                else
                {
                    if (index > 0)
                    {
//
                        //_elementInitialInfo.Locusparadcbs.Add(LOCUSPARADCB.LOCUSPARADCB(-1));
                        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                        _locusparadcb->pictureNumber = -1;
                    }
                }
            }
        }
        index++;
    }
    LBM_FreeLedBitmap(bitmapBase);
    free(_ds);
    free(_da);
    free(_dc);
    free(_ps);
    free(HaveExplosedNums);
    free(ExplosionSpeeds);
    free(IsExploseds);
    for(i=0; i<len; i++)
    {
        free(_directions[i]);

    }
    free(_directions);

    return result;
}

int DB_Babysbreath(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_Babysbreath_Create(input, output, pparams);
    return DB_Babysbreath_MakeBinaryDetail(input, output, pparams);
}
