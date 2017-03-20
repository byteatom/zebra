#include "DynamicTextEffectBase.h"

//
// <param name="dynamicTextInfo"></param>
void DT_Swirling_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{

    output->frameTotal = 10;

}

LedBitmap* DT_Swirling_ProcessBitmap(const ShineFontAreaOutput *output,const DtElementBaseParams *pparams,int index, LedBitmap *bitmap, int *px, int *py)
{
    int realWidth;
    int realHeight;
    float step;
    LedBitmap *image;

    float start;
    int distance;
    bool FReverse;
    int Result = -1;
    realWidth = bitmap->w;
    realHeight = bitmap->h;
    image = LBM_CreateLedBitmap(realWidth, realHeight,KG_BMP_32BIT);


//
    if( pparams->CurrentCharIndex==1)
    {
//
        FReverse = false;
        distance = realWidth;
    }
    else if (pparams->CurrentCharIndex==pparams->TextCount)
    {
//
        FReverse = true;
        distance = realWidth;
    }
    else if (pparams->CurrentCharIndex % 2==0)
    {
//
        FReverse = false;
        distance = realHeight;
    }
    else
    {
//
        FReverse = true;
        distance = realHeight;
    }

    if( FReverse)
    {
//
        index = output->frameTotal - index ;
    }

    step = 1.0 * distance / output->frameTotal;

    start = step * index;
    if (index == output->frameTotal)
    {
        start = distance;
    }




    if (pparams->CurrentCharIndex==1)
    {
        LBM_DrawImage(image,bitmap, LBM_MakeRect(0, 0, start, realHeight),
                      realWidth - start, 0, realWidth, realHeight);
    }
    else if (pparams->CurrentCharIndex==pparams->TextCount)
    {
        LBM_DrawImage(image,bitmap, LBM_MakeRect(start, 0, realWidth, realHeight),
                      0, 0, realWidth, realHeight);
    }
    else if (pparams->CurrentCharIndex % 2==0)
    {
//
        LBM_DrawImage(image,bitmap, LBM_MakeRect(0, 0, realWidth, start),
                      0, realHeight - start, realWidth, realHeight);
    }
    else
    {
        LBM_DrawImage(image,bitmap, LBM_MakeRect(0, start, realWidth, realHeight),
                      0, 0, realWidth, realHeight);
    }

    return image;
}


int DT_Swirling_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                              LedBitmap *bitmap, int x, int y)
{
    int result = -1;

    int i;
    int realWidth;
    int realHeight;
    float step;
    LedBitmap *image,*tmpimage;

    float width;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;

    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }

    for(i= 1; i<=output->frameTotal; i++)
    {
        realWidth = bitmap->w;
        realHeight = bitmap->h;
        step = 1.0 * realWidth / output->frameTotal;
        tmpimage = LBM_CreateLedBitmap(realWidth, realHeight,KG_BMP_32BIT);


        width = realWidth;

        LBM_DrawImage(tmpimage,bitmap, LBM_MakeRect(0, 0, width, realHeight),
                      realWidth - width, 0, realWidth, realHeight);


        image = DT_Swirling_ProcessBitmap(output,pparams,i,tmpimage,&x, &y);
        LBM_FreeLedBitmap(tmpimage);


        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);
        // Width := (Int16)image.Width, Height = (Int16)image.Height ));
        //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(image.width, image.Height));
        if (i > 1)
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i - 2);
            _locusparadcb->pictureNumber = ((pparams->CurrentCharIndex - 1) * output->frameTotal + (i - 1));
            _locusparadcb->nextPic = 1;
            _locusparadcb->offsetX = 0;
            _locusparadcb->offsetY = 0;
            //locusparadcbs[i - 2] = _locusparadcb;
        }
        LBM_FreeLedBitmap(image);
    }
    // tmplocusparadcbs := TListLOCUSPARADCB.Create();
    // Bitmaps.Reverse;




//

    _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - output->frameTotal);
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
int DT_Swirling(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                LedBitmap *bitmap, int x, int y)
{
    DT_Swirling_Create(input, output, pparams);
    return DT_Swirling_ProcessBinary(input, output, pparams,bitmap,x,y);
}

