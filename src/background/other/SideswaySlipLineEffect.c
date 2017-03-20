#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

const int SideswaySlipLine_LineWidth = 3;
int SideswaySlipLine_lineBounds;

//
// <param name="programInfo"></param>
// <param name="bounds"></param>
void DB_SideswaySlipLine_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{

    SideswaySlipLine_lineBounds = SideswaySlipLine_LineWidth + SideswaySlipLine_LineWidth * 2;
    pparams->ShapeCount = (ceil(1.0 * input->coord.width / SideswaySlipLine_lineBounds));
    pparams->IsUseCustomStartIco = true;
    pparams->IsOverrideFrameCount = true;
}
//
int DB_SideswaySlipLine_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
    LedBitmap *bitmap;

    int times;
    int i;
    int x;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;
    bitmap = LBM_CreateLedBitmap(SideswaySlipLine_LineWidth, input->coord.height,input->drawPicType);
    SDL_FillRect( bitmap, &bitmap->clip_rect, SDL_MapRGB( bitmap->format, 0xFF, 0x00, 0x00 ) );
    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);

    LBM_FreeLedBitmap(bitmap);

    times = input->coord.width*2;

    for(i=0; i<=pparams->ShapeCount-1; i++)
    {
        x = i * SideswaySlipLine_lineBounds - input->coord.width;
        _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
//
        _elementInitialInfo->startX = x;

//


        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
        _locusparadcb->nextPic = 0;
        _locusparadcb->offsetX = 1;
        _locusparadcb->offsetY = 0;
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->recycleTotal = (times - 1);

        //FrameCount = times;
        output->frameTotal = times;
    }


    return result;

}
int DB_SideswaySlipLine(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_SideswaySlipLine_Create(input, output, pparams);
    return DB_SideswaySlipLine_MakeBinaryDetail(input, output, pparams);
}
