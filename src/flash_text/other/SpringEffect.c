#include "DynamicTextEffectBase.h"

const int DT_Spring_singleCharFrameCount = 5;
//
// <param name="dynamicTextInfo"></param>
void DT_Spring_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{


    output->frameTotal = DT_Spring_singleCharFrameCount + (pparams->TextCount - 1) ;
}

int DT_Spring_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                            LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    int realHeight;
    int lastHeight;

    int i;
    int p;

    int count;

    LedBitmap *image;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;

    bool bexists;
    realHeight = bitmap->h;
    lastHeight = bitmap->h;
    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));

    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
//
    for(i=1; i<=DT_Spring_singleCharFrameCount; i++)
    {

        switch (i % DT_Spring_singleCharFrameCount)
        {
        case 0:
        case 1:
        {
//
            /*
            bexists = false;
            for(p= 0; p<=Bitmaps.Count - 1; p++)
            {
                if( Pointer(Bitmaps[p])==Pointer(bitmap))
                {
//
                    bexists = true;
                    Break;
                }
            }
            if( bexists)
            {
//
                image = LBM_Clone(bitmap);
            }
            else
            {
                image = bitmap;
            }
            */
            image = LBM_Clone(bitmap);
            break;
        }

        case 2:
        {
//
            image = DB_Base_Zoom(bitmap, 0, realHeight * 2);
            break;
        }

        case 3:
        {
//
            image = DB_Base_Zoom(bitmap, 0, realHeight * 1);
            break;
        }

        case 4:
        {
//
            image = DB_Base_Zoom(bitmap, 0, ceil(realHeight * 0.5));
            break;
        }

        default:
        {
            //raise Exception.Create('FatalError!');
            return -1;
        }

        }
        if( (((pparams->CurrentCharIndex == 1)  &&  (i > 1))  ||  (pparams->CurrentCharIndex > 1)))
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),(pparams->CurrentCharIndex-1)+(i-2));
            _locusparadcb->pictureNumber = ((pparams->CurrentCharIndex - 1) * DT_Spring_singleCharFrameCount + (i - 1));
            _locusparadcb->nextPic = 1;
            _locusparadcb->offsetY = ((lastHeight - image->h) / 2);
            //locusparadcbs[(pparams->CurrentCharIndex-1)+(i-2)] = _locusparadcb;
        }
        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);
        //Width := (Int16)image.Width, Height = (Int16)image.Height ));

        lastHeight = image->h;

        LBM_FreeLedBitmap(image);
    }

//
    count = output->frameTotal - (DT_Spring_singleCharFrameCount + (pparams->CurrentCharIndex - 1));
    for(i=0; i<=count-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),DB_Base_ShineLocusTableCount(&(_elementInitialInfo->locusTable))-count+i);
        //_locusparadcb->pictureNumber = (Int16)(pparams->CurrentCharIndex * DT_Spring_singleCharFrameCount - 1);
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
        _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - DT_Spring_singleCharFrameCount);
    }
    else
    {
        _elementInitialInfo->startPictureNumber = -1;
    }
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;


    return result;
}
int DT_Spring(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                 LedBitmap *bitmap, int x, int y)
{
    DT_Spring_Create(input, output, pparams);
    return DT_Spring_ProcessBinary(input, output, pparams,bitmap,x,y);
}
