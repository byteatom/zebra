#include "DynamicTextEffectBase.h"

//
//
void DT_Waltz_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{

    output->frameTotal = 20;
}

int DT_Waltz_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                           LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    int lastWidth;
    int lastHeight;
    int step;
    int half;

    int i;
    LedBitmap *image,*tmpimage;


    Sint16 imageId;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;
    int width;
    //Bitmaps.Add(bitmap);
    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);
    //Width := (Int16)bitmap->w, Height = (Int16)bitmap.Height ));

    lastWidth = bitmap->w;
    lastHeight = bitmap->h;
    step = output->frameTotal / 4;
    half = output->frameTotal / 2;

//
    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));

    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
    for(i=2; i<=half; i++)
    {
        //image = TLedBitmap(bitmap.Clone(MakeRect(Integer(0),Integer(0),Integer(bitmap->w),Integer(bitmap.Height)),bitmap.GetPixelFormat));


        if( i <= step)
        {
            tmpimage = LBM_Clone(bitmap);
            width = -ceil(bitmap->w * (1.0 * i / step) - 2);
        }
        else
        {
            tmpimage = LBM_RotateFlip(bitmap,Rotate180FlipY);
            width = -ceil(bitmap->w * (1.0 * (half - i) / step));
        }
        image = DB_Base_Zoom(tmpimage, width, 0);
        LBM_FreeLedBitmap(tmpimage);

        //Bitmaps.Add(image);
        //Width := (Int16)image->w, Height = (Int16)image->h ));
        //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(image->w, image->h));

        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);

//
        //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i-2);
        //_locusparadcb.PNum = ( (CurrentCharIndex - 1) * half + (i - 1));
        _locusparadcb->pictureNumber = ( (pparams->CurrentCharIndex - 1) * half + (i - 1));
        //_locusparadcb.BNext = 1;
        _locusparadcb->nextPic = 1;
        //_locusparadcb.XOffset = ((lastWidth - image->w) / 2);
        _locusparadcb->offsetX = ((lastWidth - image->w) / 2);
        //_locusparadcb.YOffset = ((lastHeight -image->h) / 2);
        _locusparadcb->offsetY = ((lastHeight -image->h) / 2);

        lastWidth = image->w;
        lastHeight = image->h;
        //locusparadcbs[i-2] = _locusparadcb;
        LBM_FreeLedBitmap(image);
    }
//
    for(i=0; i<=half - 2; i++)
    {
//
        imageId = ( pparams->CurrentCharIndex * half - (i + 2)) ;
        //image = Bitmaps[imageId];
        image = DB_Base_ShinePictureTableAt(&(pparams->Bitmaps),imageId);
//
        //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),half+(i-1));
        //_locusparadcb.PNum = imageId;
        _locusparadcb->pictureNumber = imageId;
        //_locusparadcb.BNext = 1;
        _locusparadcb->nextPic = 1;
        //_locusparadcb.XOffset = ((lastWidth - image->w) / 2);
        _locusparadcb->offsetX = ((lastWidth - image->w) / 2);
        //_locusparadcb.YOffset = ((lastHeight - image->h) / 2);
        _locusparadcb->offsetY = ((lastHeight - image->h) / 2);
        lastWidth = image->w;
        lastHeight = image->h;
        //locusparadcbs[half+(i-1)] = _locusparadcb;
    }

    //Locusparadcbses.Add(locusparadcbs);
//
    //_elementparadcb = ELEMENTPARADCB.ELEMENTPARADCB(0);
    //_elementparadcb.LocusNum = locusparadcbs.Count;
    //_elementparadcb.StartIco = (Icoparadcbs.Count - half);
    _elementInitialInfo->startPictureNumber = DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - half;
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    //Elementparadcbs.Add(_elementparadcb);
    return result;

}
int DT_Waltz(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
             LedBitmap *bitmap, int x, int y)
{
    DT_Waltz_Create(input, output, pparams);
    return DT_Waltz_ProcessBinary(input, output, pparams,bitmap,x,y);
}
