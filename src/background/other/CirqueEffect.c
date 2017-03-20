
#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"



void DB_Cirque_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    pparams->GroupCount = 1;
    pparams->ShapeCount = (ceil((input->coord.width * input->coord.height) / (1024.0 * 4)));
    pparams->CurrentLoopIndex = 0;
    pparams->LoopCount = pparams->ShapeCount * 4;
    pparams->BitmapInfoDictionaries.isnull = true;
    pparams->BitmapInfoDictionaries.next = NULL;
    pparams->IsUseCustomStartIco = true;
    pparams->Elements.isnull = true;
    pparams->Elements.next = NULL;
    pparams->Bitmaps.isnull = true;
    pparams->Bitmaps.next = NULL;
}

//
int DB_Cirque_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
//
    //_dcolors: array[0..2] of TColor = ((clRed),
    //  (clGreen),(clYellow));
//
    const int _dx[8] = {0,1,1,1,0,-1,-1,-1};
//
    const int _dy[8] = {-1,-1,0,1,1,1,0,-1};






    int index;
    int i;
    char t[STR_BUF_COUNT] = "";
    int tm;
    int x;
    int y;
    int d;

//
    int *_ds = (int *)malloc(pparams->ShapeCount*sizeof(int));
//
    LBM_Point *_ps = (LBM_Point *)malloc(pparams->ShapeCount*sizeof(LBM_Point));
//
//
    char **_directions = (char **)malloc(pparams->ShapeCount*sizeof(char *));//[STR_BUF_COUNT];

    LedBitmap *bitmap;

    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;

    int len;



    len = pparams->ShapeCount*pparams->GroupCount;


    for(i=0; i<=pparams->ShapeCount-1; i++)
    {
        _directions[i] = (char *)malloc(STR_BUF_COUNT);
        memset(_directions[i],0,STR_BUF_COUNT);
        _ds[i] = 0;
        _ps[i] = LBM_MakePoint(0,0);
    }
//
    index = 0;
    while((pparams->CurrentLoopIndex < pparams->LoopCount))
    {
        for(i=0; i<=pparams->ShapeCount-1; i++)
        {
            if (_ds[i] <= 0)
            {
                pparams->CurrentLoopIndex++;
                _ds[i] = DB_Base_GetRandom(32);
                _ps[i] = LBM_MakePoint(DB_Base_GetRandom(input->coord.width), DB_Base_GetRandom(input->coord.height));
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
            //tm = atoi(direction[0]);
            tm = _directions[i][0]-'0';
            //_directions[i] := _directions[i].Remove(0, 1);
            String_Delete(_directions[i],1,1);
            _ps[i].X = _ps[i].X+_dx[tm] * 3;
            _ps[i].Y = _ps[i].Y+_dy[tm] * 3;
            _ds[i] = _ds[i]-(DB_Base_GetRandom(2) + 1);
//
            x = _ps[i].X - _ds[i];
            y = _ps[i].Y - _ds[i];
//
            d = _ds[i]*2;
            if (d <= 0)
            {
                d = 0;
            }
            if( !MapIntInt_ContainsKey(&(pparams->BitmapInfoDictionaries),d))
            {
                if(d > 0)
                {
                    bitmap = LBM_CreateLedBitmap(d, d, input->drawPicType);

                    //graphics.Clear(TColorFilter.Instance.RealTransparentColor.AColor);

                    //gppen = TGPPen.Create(TLedColor.TLedColor(clRed).AColor);
                    //graphics.DrawEllipse(gppen, 0, 0, d - 1, d - 1);
                    circleColor(bitmap,d/2,d/2,d/2-1,LBM_KGColorToSdlColor(KG_COLOR_RED));

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

                //_elementInitialInfo.LastX = x;
                //_elementInitialInfo.StartX = x;
                //_elementInitialInfo.LastY = y;
                //_elementInitialInfo.StartY = y;
                //_elementInitialInfo.StartIco = MapIntInt_ValueAt(BitmapInfoDictionaries,d);
                //ElementInitialInfos[i] = _elementInitialInfo;
            }
            if(d > 0)
            {
//
                if(index > 0)
                {
                    //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);;
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
                    //_locusparadcb.PNum = MapIntInt_ValueAt(BitmapInfoDictionaries,d);
                    //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb);
                    //_elementInitialInfo.LastX = x;
                    //_elementInitialInfo.LastY = y;
                }
            }
            else
            {
                if(index > 0)
                {
//
                    //PNum := -1 ));
                    //_elementInitialInfo.Locusparadcbs.Add(LOCUSPARADCB.LOCUSPARADCB(-1));
                    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                    _locusparadcb->pictureNumber = -1;
                }
            }
        }
        index++;
    }

    MapIntInt_Free(&(pparams->BitmapInfoDictionaries));
    free(_ds);
    free(_ps);
    for(i=0; i<=pparams->ShapeCount-1; i++)
    {
        free(_directions[i]);
    }
    free(_directions);
    return result;
}

int DB_Cirque(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_Cirque_Create(input, output, pparams);
    return DB_Cirque_MakeBinaryDetail(input, output, pparams);
}
