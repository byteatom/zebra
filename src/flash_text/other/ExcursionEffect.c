#include "DynamicTextEffectBase.h"

//
// <param name="dynamicTextInfo"></param>
void DT_Excursion_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{

    output->frameTotal = 10;
    //FReverse = False;
}



int DT_Excursion_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                                LedBitmap *bitmap, int x, int y,bool FReverse)
{
    int result = -1;
    int i;
    int realWidth;
    int realHeight;
    float step;
    LedBitmap *image;

    float width;
    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;

    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));

    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
    for(i= 1; i<=output->frameTotal; i++)
        // for i :=output->frameTotal downto 1  do
    {
        realWidth = bitmap->w;
        realHeight = bitmap->h;
        step = 1.0 * realWidth / output->frameTotal;
        image = LBM_CreateLedBitmap(realWidth, realHeight,KG_BMP_32BIT);


        width = step * i;
        if (i == output->frameTotal)
        {
            width = realWidth;
        }
        //graphics.Clear(TColorFilter.Instance.RealTransparentColor.AColor);
        //graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
        if( FReverse)
        {
            LBM_DrawImage(image,bitmap, LBM_MakeRect(realWidth - width, 0, width, realHeight),
                               0, 0, width, realHeight);
        }
        else
        {
            LBM_DrawImage(image,bitmap, LBM_MakeRect(0, 0, width, realHeight),
                               realWidth - width, 0, realWidth, realHeight);
        }
        //graphics.DrawImage(bitmap, MakeRect(0, 0, width, realHeight),
        //  realWidth - width, 0, realWidth, realHeight, TUnit.UnitPixel);

        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);
        // Width := (Int16)image.Width, Height = (Int16)image.Height ));
        //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(image.width, image.Height));
        if (i > 1)
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i-2);
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
    if( FReverse)
    {
//
        //locusparadcbs.Reverse();
    }

    //Locusparadcbses.Add(locusparadcbs);

//
    //_elementparadcb = ELEMENTPARADCB.ELEMENTPARADCB(0);
    //_elementparadcb.LocusNum = locusparadcbs.Count;
    _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - output->frameTotal);
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
int DT_Excursion(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
             LedBitmap *bitmap, int x, int y)
{
    DT_Excursion_Create(input, output, pparams);
    return DT_Excursion_ProcessBinary(input, output, pparams,bitmap,x,y,false);
}
