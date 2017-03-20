#include "DynamicTextEffectBase.h"

const int DT_RollOver_singleCharFrameCount = 20;
//
// <param name="dynamicTextInfo"></param>
void DT_RollOver_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{
    output->frameTotal = DT_RollOver_singleCharFrameCount + (pparams->TextCount - 1);
}

int DT_RollOver_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                              LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    int realWidth;
    int realHeight;
    int lastWidth;
    int lastHeight;
    int step;
    int half;

    int i;
    int p;
    int change;
    LedBitmap *clonedBitmap;

    int count;

    LedBitmap *image;

    int height;
    bool bexists;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;

    realWidth = bitmap->w;
    realHeight = bitmap->h;
    lastWidth = realWidth;
    lastHeight = realHeight;
    step = DT_RollOver_singleCharFrameCount / 4;
    half = DT_RollOver_singleCharFrameCount / 2;
    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));

    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
    for(i=1; i<=DT_RollOver_singleCharFrameCount; i++)
    {


        change = i % DT_RollOver_singleCharFrameCount;
        if (change <= 1)
        {
//
            /*
            bexists = False;
            for(p= 0; p<=Bitmaps.Count - 1; p++)
            {
                if( Pointer(Bitmaps[p])==Pointer(bitmap))
                {
//
                    bexists = True;
                    Break;
                }
            }
            if( bexists)
            {
//
                image = TLedBitmap(bitmap.Clone(MakeRect(Integer(0),Integer(0),Integer(bitmap.Width),Integer(bitmap.Height)),bitmap.GetPixelFormat));
            }
            else
            {
                image = bitmap;
            }
            */
            image = LBM_Clone(bitmap);
        }
        else if ((change > 1)  &&  (change <= step))
        {
            height = -ceil(realHeight * (1.0 * change / step) - 2);
            image = DB_Base_Zoom(bitmap, 0, height);
        }
        else if ((change > step)  &&  (change <= half))
        {
            clonedBitmap = LBM_RotateFlip(bitmap,Rotate180FlipX);
            height = -ceil(realHeight * (1.0 * (half - change) / step));
            image = DB_Base_Zoom(clonedBitmap, 0, height);
            LBM_FreeLedBitmap(clonedBitmap);
        }
        else if ((change > half)  &&  (change <= (half + step)))
        {
            clonedBitmap = LBM_RotateFlip(bitmap,Rotate180FlipX);
            height = -ceil(realHeight * (1.0 * (change - half) / step) - 2);
            image = DB_Base_Zoom(clonedBitmap, 0, height);
            LBM_FreeLedBitmap(clonedBitmap);
        }
        else
        {
            height = -ceil(realHeight * (1.0 * (DT_RollOver_singleCharFrameCount - change) / step));
            image = DB_Base_Zoom(bitmap, 0, height);
        }
        if( (((pparams->CurrentCharIndex == 1)  &&  (i > 1))  ||  (pparams->CurrentCharIndex > 1)))
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),(pparams->CurrentCharIndex-1)+(i-2));
            _locusparadcb->pictureNumber = ((pparams->CurrentCharIndex - 1) * DT_RollOver_singleCharFrameCount + (i - 1));
            _locusparadcb->nextPic = 1;
            _locusparadcb->offsetX = ((lastWidth - image->w) / 2);
            _locusparadcb->offsetY = ((lastHeight - image->h) / 2);
            //locusparadcbs[(pparams->CurrentCharIndex-1)+(i-2)] = _locusparadcb;
        }
        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);
        //Width := (Int16)image.Width, Height = (Int16)image.Height ));

        lastWidth = image->w;
        lastHeight = image->h;

        LBM_FreeLedBitmap(image);
    }

//
    count = output->frameTotal - (DT_RollOver_singleCharFrameCount + (pparams->CurrentCharIndex - 1));
    for(i=0; i<=count-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),DB_Base_ShineLocusTableCount(&(_elementInitialInfo->locusTable))-count+i);
        //_locusparadcb->pictureNumber = (Int16)(pparams->CurrentCharIndex * DT_RollOver_singleCharFrameCount - 1);
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->nextPic = 0;
        //locusparadcbs[locusparadcbs.Count-count+i] = _locusparadcb;
    }
//
    for(i=0; i<=(pparams->CurrentCharIndex - 2)-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i);
        _locusparadcb->pictureNumber = -1;
        _locusparadcb->nextPic = 0;
        //locusparadcbs[i] = _locusparadcb;
    }
//

//
    if (pparams->CurrentCharIndex == 1)
    {
//
        _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - DT_RollOver_singleCharFrameCount);
    }
    else
    {
        _elementInitialInfo->startPictureNumber = -1;
    }
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
int DT_RollOver(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                LedBitmap *bitmap, int x, int y)
{
    DT_RollOver_Create(input, output, pparams);
    return DT_RollOver_ProcessBinary(input, output, pparams,bitmap,x,y);
}
