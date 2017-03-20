#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

//
const int SingleScan_LineWidth = 4;


//
// <param name="programInfo"></param>
// <param name="bounds"></param>
void DB_SingleScan_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{


    pparams->ShapeCount = 2;
    pparams->IsUseCustomStartIco = true;
    pparams->IsOverrideFrameCount = true;
}
//
int DB_SingleScan_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
    LedBitmap *bitmap;

    int times;
    int i;
    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;

    bitmap = LBM_CreateLedBitmap(SingleScan_LineWidth, input->coord.height,input->drawPicType);
    SDL_FillRect( bitmap, &bitmap->clip_rect, SDL_MapRGB( bitmap->format, 0xFF, 0x00, 0x00 ) );

    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);



    times = input->coord.width - bitmap->w;


    for(i=0; i<=pparams->ShapeCount-1; i++)
    {
        _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
        _elementInitialInfo->startX =  (i*(input->coord.width - bitmap->w));

        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));

        _locusparadcb->nextPic = 0;
        if (i == 0)
        {
            _locusparadcb->offsetX = 1;
        }
        else
        {
            _locusparadcb->offsetX = -1;
        }
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->recycleTotal = (times - 1);


    }
    output->frameTotal = times;

    LBM_FreeLedBitmap(bitmap);
    return result;

}
int DB_SingleScan(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_SingleScan_Create(input, output, pparams);
    return DB_SingleScan_MakeBinaryDetail(input, output, pparams);
}

