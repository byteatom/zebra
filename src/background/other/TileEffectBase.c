#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"
#include "TileEffectBase.h"

//
// <param name="programInfo"></param>
// <param name="bounds"></param>
void DB_Tile_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams, TileBaseParams *tileParams)
{

    tileParams->MaxD = 32;
    tileParams->FZoomMode = tzmOut;
    if( (input->coord.width < tileParams->MaxD) || (input->coord.height < tileParams->MaxD))
    {
        // tileParams->MaxD = input->coord.width / 2;
        tileParams->MaxD = min(input->coord.width, input->coord.height);
    }
    tileParams->FColumnCount = input->coord.width / tileParams->MaxD;
    tileParams->FRowCount = input->coord.height / tileParams->MaxD;
    pparams->ShapeCount = tileParams->FColumnCount * tileParams->FRowCount;
    pparams->IsUseCustomStartIco = true;
}

LedBitmap* DB_Tile_GetGraphicsBitmap(TileBaseParams *tileParams, int index, LBM_RectF *prectangleF, int *pelementIndex, KGPicType pictype, pFGetGraphicsPath GetGraphicsPath)
{
    LedBitmap *bitmap;
    *pelementIndex = 0;


    bitmap = LBM_CreateLedBitmap(tileParams->MaxD, tileParams->MaxD,pictype);
    GetGraphicsPath(tileParams,bitmap,index,prectangleF,KG_COLOR_RED);

    return bitmap;
}

//
int DB_Tile_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams, TileBaseParams *tileParams,pFGetGraphicsBitmap GetGraphicsBitmap, pFGetGraphicsPath GetGraphicsPath)
{
    int result = -1;
    int spaccing;
    int index;
    int currentWidth;

    LBM_RectF rectangleF;
    int d;
    LedBitmap *bitmap;

    int temp;
    int i;
    int j;
    int x;
    int y;
    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;
    int elementIndex;

    pFGetGraphicsBitmap fGetGraphicsBitmap = (GetGraphicsBitmap!=NULL)?GetGraphicsBitmap:DB_Tile_GetGraphicsBitmap;
    spaccing = tileParams->MaxD / 4;
//
    index = 0;
    currentWidth = 0;
    elementIndex = -1;
    while((currentWidth < (tileParams->MaxD - spaccing)))
    {


        bitmap = fGetGraphicsBitmap(tileParams, index, &rectangleF,&elementIndex,input->drawPicType,GetGraphicsPath);
        currentWidth = ceil(rectangleF.Width);
        if (tileParams->FZoomMode == tzmIn)
        {
//
            LBM_FreeLedBitmap(bitmap);
        }
        else
        {
            // rectangleF := graphicsPath.GetBounds();
            d = ceil(rectangleF.Width);
            if( !MapIntInt_ContainsKey(&(pparams->BitmapInfoDictionaries),d))
            {
                if (d > 0)
                {


                    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);
                    LBM_FreeLedBitmap(bitmap);

                }
                else
                {
                    LBM_FreeLedBitmap(bitmap);
                    result = DB_Base_AddPlaceHolderBitmap(&(pparams->Bitmaps),input->dir,input->drawPicType,input->createMode);
                }
                MapIntInt_Add(&(pparams->BitmapInfoDictionaries),d, DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - 1);
            }
            else
            {
                LBM_FreeLedBitmap(bitmap);
            }
            temp = 0;
            for(i= 0; i<=tileParams->FColumnCount - 1; i++)
            {
                for(j= 0; j<=tileParams->FRowCount - 1; j++)
                {
                    x = i * tileParams->MaxD;
                    y = j * tileParams->MaxD;
                    _elementInitialInfo = DB_Base_ShineElementTableAt(&(pparams->Elements),temp);
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

                        //_elementInitialInfo.StartX = x;
                        //_elementInitialInfo.StartY = y;
                        //_elementInitialInfo.StartIco = BitmapInfoDictionaries[d];
                        //ElementInitialInfos[temp] = _elementInitialInfo;
                    }
                    if (d > 0)
                    {
//
                        if (index > 0)
                        {
                            // BNext := 1, PNum = (Int16)BitmapInfoDictionaries[d] ));
                            //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
                            _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                            _locusparadcb->nextPic = 1;
                            _locusparadcb->offsetX = 0;
                            _locusparadcb->offsetY = 0;
                            _locusparadcb->pictureNumber = MapIntInt_ValueAt(&(pparams->BitmapInfoDictionaries),d);
                            _locusparadcb->recycleTotal = 0;
                            _elementInitialInfo->lastX = x;
                            _elementInitialInfo->lastY = y;

                            //_locusparadcb.BNext = 1;
                            //_locusparadcb.PNum = BitmapInfoDictionaries[d];
                            //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb); // new LOCUSPARADCB { BNext == 1, PNum == (Int16)BitmapInfoDictionaries[d] });
                            //_elementInitialInfo.LastX = x;
                            //_elementInitialInfo.LastY = y;
                        }
                    }
                    else
                    {
                        if (index > 0)
                        {
//
                            // PNum := -1 ));
                            //_elementInitialInfo.Locusparadcbs.Add
                            //(LOCUSPARADCB.LOCUSPARADCB(-1));
                            _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                            _locusparadcb->pictureNumber = -1;
                        }
                    }
                    temp++;
                }
            }
        }
        index++;
    }
    if ((tileParams->FZoomMode == tzmIn) || (tileParams->FZoomMode == tzmOutIn))
    {
//
        while((index > 0))
        {

            bitmap = fGetGraphicsBitmap(tileParams, index, &rectangleF,&elementIndex,input->drawPicType,GetGraphicsPath);
            currentWidth = ceil(rectangleF.Width);
            // rectangleF := graphicsPath.GetBounds();
            d = ceil(rectangleF.Width);
            if( !MapIntInt_ContainsKey(&(pparams->BitmapInfoDictionaries),d))
            {
                if (d > 0)
                {


                    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);
                    LBM_FreeLedBitmap(bitmap);

                }
                else
                {
                    LBM_FreeLedBitmap(bitmap);
                    result = DB_Base_AddPlaceHolderBitmap(&(pparams->Bitmaps),input->dir,input->drawPicType,input->createMode);
                }
                MapIntInt_Add(&(pparams->BitmapInfoDictionaries),d, DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - 1);
            }
            else
            {
                LBM_FreeLedBitmap(bitmap);
            }
            temp = 0;
            for(i= 0; i<=tileParams->FColumnCount - 1; i++)
            {
                for(j= 0; j<=tileParams->FRowCount - 1; j++)
                {
                    x = i * tileParams->MaxD;
                    y = j * tileParams->MaxD;
                    _elementInitialInfo = DB_Base_ShineElementTableAt(&(pparams->Elements),temp);
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
                        //_elementInitialInfo.StartX = x;
                        //_elementInitialInfo.StartY = y;
                        //_elementInitialInfo.StartIco = BitmapInfoDictionaries[d];
                        //ElementInitialInfos[temp] = _elementInitialInfo;
                    }
                    if (d > 0)
                    {
//
                        if (index > 0)
                        {
                            // BNext := 1, PNum = (Int16)BitmapInfoDictionaries[d] ));
                            //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
                            _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                            _locusparadcb->nextPic = 1;
                            _locusparadcb->offsetX = 0;
                            _locusparadcb->offsetY = 0;
                            _locusparadcb->pictureNumber = MapIntInt_ValueAt(&(pparams->BitmapInfoDictionaries),d);
                            _locusparadcb->recycleTotal = 0;
                            _elementInitialInfo->lastX = x;
                            _elementInitialInfo->lastY = y;

                            //_locusparadcb.BNext = 1;
                            //_locusparadcb.PNum = BitmapInfoDictionaries[d];
                            //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb); // new LOCUSPARADCB { BNext == 1, PNum == (Int16)BitmapInfoDictionaries[d] });
                            //_elementInitialInfo.LastX = x;
                            //_elementInitialInfo.LastY = y;
                        }
                    }
                    else
                    {
                        if (index > 0)
                        {
//
                            // PNum := -1 ));
                            //_elementInitialInfo.Locusparadcbs.Add
                            //(LOCUSPARADCB.LOCUSPARADCB(-1));
                            _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
                            _locusparadcb->pictureNumber = -1;
                        }
                    }
                    temp++;
                }
            }
            index++;
        }
    }
    return result;
}
