#include "DynamicTextEffectBase.h"

//
// <param name="dynamicTextInfo"></param>
const int DT_RadioWave_singleCharFrameCount = 4;
void DT_RadioWave_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{


    output->frameTotal = DT_RadioWave_singleCharFrameCount * pparams->TextCount;
}

int DT_RadioWave_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                               LedBitmap *bitmap, int x, int y)
{
    int result = -1;

    KGColor color;
    int size;
    int centerX;
    int startY;

    int i;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;
    LedBitmap *image;
    if(input->colorMode==FONT_COLOR_MODE_RANDOM)
    {
//
        color = input->colors[pparams->CurrentCharIndex-1];
    }
    else
    {
        color = input->colors[0];//TLedColor.TLedColor(FDynamicText.Font.Color.Color);
    }
    size = min(bitmap->w, bitmap->h);
    centerX = bitmap->w / 2;
    startY = (bitmap->h - size) / 2;

    //__memset(@_locusparadcb,0,sizeof(LOCUSPARADCB));

    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
//
    for(i=1; i<=DT_RadioWave_singleCharFrameCount; i++)
    {

        switch (i % DT_RadioWave_singleCharFrameCount)
        {
        case 0:
        {
//
            image = LBM_Clone(bitmap);
            break;
        }

        case 1:
        {
//
            image = LBM_CreateLedBitmap(bitmap->w, bitmap->h,KG_BMP_32BIT);


            LBM_FillEllipse(image, LBM_MakeRect(0, startY, size, size),color);

            break;
        }

        case 2:
        {
//
            image = LBM_CreateLedBitmap(bitmap->w, bitmap->h,KG_BMP_32BIT);
            LBM_FillEllipse(image, LBM_MakeRect(size / 4, startY, size / 2, size),color);

            break;
        }
        case 3:
        {
//
            image = LBM_CreateLedBitmap(bitmap->w, bitmap->h,KG_BMP_32BIT);
            thickLineColor(image, centerX, 0, centerX, bitmap->h,2,LBM_KGColorToSdlColor(color));

            break;
        }
        default:
        {
            //raise Exception.Create('FatalError!');
            return -1;
        }

        }
        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,image,input->createMode);
        //Width := (Int16)image.Width, Height = (Int16)image.Height ));
        //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(image.Width, image.Height));
        if(( ( (pparams->CurrentCharIndex == 1)   &&  (i > 1) )   ||  (pparams->CurrentCharIndex > 1) ))
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),((pparams->CurrentCharIndex-1)*DT_RadioWave_singleCharFrameCount)+(i-2));
            _locusparadcb->pictureNumber = ((pparams->CurrentCharIndex - 1) * DT_RadioWave_singleCharFrameCount + (i - 1));
            _locusparadcb->nextPic = 1;
            //locusparadcbs[((pparams->CurrentCharIndex-1)*DT_RadioWave_singleCharFrameCount)+(i-2)] = _locusparadcb;
        }
        LBM_FreeLedBitmap(image);
    }

//
    for(i=0; i<=output->frameTotal - DT_RadioWave_singleCharFrameCount * pparams->CurrentCharIndex-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),((pparams->CurrentCharIndex-1)*DT_RadioWave_singleCharFrameCount)+(i+(DT_RadioWave_singleCharFrameCount-1)));
        //_locusparadcb.PNum = (Int16)(pparams->CurrentCharIndex * DT_RadioWave_singleCharFrameCount - 1);
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->nextPic = 0;
        //locusparadcbs[((pparams->CurrentCharIndex-1)*DT_RadioWave_singleCharFrameCount)+(i+(DT_RadioWave_singleCharFrameCount-1))] = _locusparadcb;
    }
//
    if(pparams->CurrentCharIndex > 1)
    {
        for(i=0; i<=output->frameTotal - (output->frameTotal / DT_RadioWave_singleCharFrameCount - (pparams->CurrentCharIndex - 1)) * DT_RadioWave_singleCharFrameCount - 1-1; i++)
        {
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i);
            _locusparadcb->pictureNumber = -1;
            _locusparadcb->nextPic = 0;
            //locusparadcbs[i] = _locusparadcb;
        }
    }
//

//
    if(pparams->CurrentCharIndex == 1)
    {
        _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - DT_RadioWave_singleCharFrameCount);
    }
    else
    {
        _elementInitialInfo->startPictureNumber = -1;
    }
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
int DT_RadioWave(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                 LedBitmap *bitmap, int x, int y)
{
    DT_RadioWave_Create(input, output, pparams);
    return DT_RadioWave_ProcessBinary(input, output, pparams,bitmap,x,y);
}
