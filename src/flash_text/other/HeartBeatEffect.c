#include "LedBitmap.h"
#include "DynamicTextEffectBase.h"
#include "Common.h"

//
//

void DT_HeartBeat_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{

    output->frameTotal = 2;
}

int DT_HeartBeat_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                               LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    int realWidth;
    int realHeight;
    LedBitmap *secondBitmap;
    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;
    //Bitmaps.Add(bitmap);
    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);
//
    realWidth = bitmap->w;
    realHeight = bitmap->h;

//
    secondBitmap = DB_Base_Zoom(bitmap, -realWidth / 2, -realHeight / 2);


    //Bitmaps.Add(secondBitmap);
    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,secondBitmap,input->createMode);

    //

    //Width := (Int16)bitmap.Width, Height = (Int16)bitmap.Height ));
    //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(bitmap.Width, bitmap.Height));
    //Width := (Int16)secondBitmap.Width, Height = (Int16)secondBitmap.Height ));
    //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(secondBitmap.Width, secondBitmap.Height));

    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
//
    //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
    //_locusparadcb.PNum = (Icoparadcbs.Count - 1);
    //_locusparadcb.BNext = 1;
    // _locusparadcb.XOffset = ShortInt((bitmap.Width - secondBitmap.Width) / 2);
    //_locusparadcb.YOffset = ShortInt((bitmap.Height - secondBitmap.Height) / 2);
    //locusparadcbs = TListLOCUSPARADCB.Create();
    //locusparadcbs.Add(_locusparadcb);
    //Locusparadcbses.Add(locusparadcbs);

    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));

    _locusparadcb->nextPic = 1;
    _locusparadcb->offsetX = (bitmap->w - secondBitmap->w) / 2;
    _locusparadcb->offsetY = (bitmap->h - secondBitmap->h) / 2;

    _locusparadcb->pictureNumber = DB_Base_ShinePictureTableCount(&(pparams->Bitmaps))-1;

    _locusparadcb->recycleTotal = 0;

//
    //_elementparadcb = ELEMENTPARADCB.ELEMENTPARADCB(0);
    //_elementparadcb.LocusNum = locusparadcbs.Count;
    //_elementparadcb.StartIco = (Icoparadcbs.Count - FrameCount);
    //_elementparadcb.StartX = x;
    //_elementparadcb.StartY = y;
    //Elementparadcbs.Add(_elementparadcb);

    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;

    _elementInitialInfo->startPictureNumber = DB_Base_ShinePictureTableCount(&(pparams->Bitmaps))-output->frameTotal;



    LBM_FreeLedBitmap(secondBitmap);
    return result;
}

int DT_HeartBeat(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                 LedBitmap *bitmap, int x, int y)
{
    DT_HeartBeat_Create(input, output, pparams);
    return DT_HeartBeat_ProcessBinary(input, output, pparams,bitmap,x,y);
}



