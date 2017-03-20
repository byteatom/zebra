#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

const int FaceToFaceSlipLine_LineWidth = 3;
int FaceToFaceSlipLine_lineBounds;


void DB_FaceToFaceSlipLine_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{

    FaceToFaceSlipLine_lineBounds = 8;
    pparams->ShapeCount = ceil(1.0*input->coord.width/FaceToFaceSlipLine_lineBounds);
    pparams->IsUseCustomStartIco = true;
    pparams->IsOverrideFrameCount = true;
}
//
int DB_FaceToFaceSlipLine_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
    LedBitmap *bitmap;

    int times;
    int half;
    int i;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;
    int x;
    int offsetX;
    bitmap = LBM_CreateLedBitmap(FaceToFaceSlipLine_LineWidth, input->coord.height,input->drawPicType);

    SDL_FillRect( bitmap, &bitmap->clip_rect, SDL_MapRGB( bitmap->format, 0xFF, 0x00, 0x00 ) );
    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);

    LBM_FreeLedBitmap(bitmap);
//
    //int times = input->coord.width/2;
    times = input->coord.width * 2 - (FaceToFaceSlipLine_lineBounds - FaceToFaceSlipLine_LineWidth) * (pparams->ShapeCount - 1);
    half = pparams->ShapeCount / 2;
    for(i=0; i<=pparams->ShapeCount-1; i++)
    {

        if (i < half)
        {
            x = i* FaceToFaceSlipLine_lineBounds - input->coord.width / 2 + (FaceToFaceSlipLine_lineBounds - FaceToFaceSlipLine_LineWidth);
            offsetX = 1;
        }
        else
        {
            x = (i - half) * FaceToFaceSlipLine_lineBounds + input->coord.width;
            offsetX = -1;
        }
        _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
//
        _elementInitialInfo->startX =  x;

//


        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
        _locusparadcb->nextPic = 0;
        _locusparadcb->offsetX = offsetX;
        _locusparadcb->offsetY = 0;
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->recycleTotal = (times - 1);

        //FrameCount = times;
        output->frameTotal = times;
    }
    return result;

}
int DB_FaceToFaceSlipLine(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_FaceToFaceSlipLine_Create(input, output, pparams);
    return DB_FaceToFaceSlipLine_MakeBinaryDetail(input, output, pparams);
}
