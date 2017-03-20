#include "DynamicTextEffectBase.h"

const int DT_Domino_singleCharFrameCount = 13;
//
// <param name="dynamicTextInfo"></param>
void DT_Domino_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{
    output->frameTotal = DT_Domino_singleCharFrameCount + (pparams->TextCount - 1) * 2;
}

int DT_Domino_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                            LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    double _angleStep = 15.0;

    int step;
    int i;
    int p;
    int change;

    int count;

    LedBitmap *image,*tmpimage;
    //bool bexists;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;

    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }

    step = DT_Domino_singleCharFrameCount / 2;
    for(i=1; i<=DT_Domino_singleCharFrameCount; i++)
    {

        change = i % DT_Domino_singleCharFrameCount;

        if( (change > 1)  &&  (change <= 4))
        {
//
            image = LBM_Rotation(bitmap, (change - 1) * _angleStep);
        }
        else if ((change > 4)  &&  (change <= 6))
        {
//
            image = LBM_Rotation(bitmap, (step - change + 1) * _angleStep);
        }
        else if ((change > 7)  &&  (change <= 10))
        {
//
            image = LBM_Rotation(bitmap, (step - change + 1) * _angleStep);
        }
        else if ((change > 10)  &&  (change <= 12))
        {
//
            image = LBM_Rotation(bitmap, (change - DT_Domino_singleCharFrameCount) * _angleStep);
        }
        else
        {
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
        if(image->w>bitmap->w)
        {
            tmpimage = LBM_CreateLedBitmap(bitmap->w,bitmap->h,KG_BMP_32BIT);
            LBM_Draw(tmpimage,(bitmap->w-image->w)/2,(bitmap->h-image->h)/2,image);
            LBM_FreeLedBitmap(image);
            image = tmpimage;
        }
        if( (((pparams->CurrentCharIndex == 1)  &&  (i > 1))  ||  (pparams->CurrentCharIndex > 1)))
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),(pparams->CurrentCharIndex-1)*2+(i-2));
            _locusparadcb->pictureNumber = ((pparams->CurrentCharIndex - 1) * DT_Domino_singleCharFrameCount + (i - 1));
            _locusparadcb->nextPic = 1;
            //locusparadcbs[(pparams->CurrentCharIndex-1)*2+(i-2)] = _locusparadcb;
        }
        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);

        //Width := (Int16)image.Width, Height = (Int16)image.Height ));
        //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(image.Width, image.Height));
        LBM_FreeLedBitmap(image);
    }

//
    count = output->frameTotal - (DT_Domino_singleCharFrameCount + (pparams->CurrentCharIndex - 1) * 2);
    for(i=0; i<=count-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),DB_Base_ShineLocusTableCount(&(_elementInitialInfo->locusTable))-count+i);
        //_locusparadcb->pictureNumber = (Int16)(Icoparadcbs.Count - DT_Domino_singleCharFrameCount);
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->nextPic = 0;
        //locusparadcbs[locusparadcbs.Count-count+i] = _locusparadcb;
    }
//
    for(i=0; i<=(pparams->CurrentCharIndex - 1 ) * 2-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i);
        //_locusparadcb->pictureNumber = (Int16) (Icoparadcbs.Count - DT_Domino_singleCharFrameCount);
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->nextPic = 0;
        //locusparadcbs[i] = _locusparadcb;
    }
//

    _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - DT_Domino_singleCharFrameCount);
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
int DT_Domino(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
              LedBitmap *bitmap, int x, int y)
{
    DT_Domino_Create(input, output, pparams);
    return DT_Domino_ProcessBinary(input, output, pparams,bitmap,x,y);
}
