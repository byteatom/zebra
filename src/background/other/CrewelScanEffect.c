#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

//
const int CrewelScan_LineWidth = 4;
//
// <param name="programInfo"></param>
// <param name="bounds"></param>
void DB_CrewelScan_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{


    pparams->ShapeCount = 1;
    pparams->IsUseCustomStartIco = true;
    pparams->IsOverrideFrameCount = true;
}
//
int DB_CrewelScan_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
    LedBitmap *bitmap;

    int times;
    ShineElementTable *_elementInitialInfo;

    ShineLocusTable *_locusparadcb;
    bitmap = LBM_CreateLedBitmap(CrewelScan_LineWidth * 3, input->coord.height,input->drawPicType);


    rectangleColor(bitmap, 0, 0, CrewelScan_LineWidth, input->coord.height,LBM_KGColorToSdlColor(KG_COLOR_RED));
    rectangleColor(bitmap, CrewelScan_LineWidth * 2, 0, CrewelScan_LineWidth, input->coord.height,LBM_KGColorToSdlColor(KG_COLOR_RED));

    result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,bitmap,input->createMode);


    times = (input->coord.width - bitmap->w) * 2;


//
    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));

//
//


    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    _locusparadcb->nextPic = 0;
    _locusparadcb->offsetX = 1;
    _locusparadcb->offsetY = 0;
    _locusparadcb->pictureNumber = 0;
    _locusparadcb->recycleTotal = (times / 2 - 1);



    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    _locusparadcb->nextPic = 0;
    _locusparadcb->offsetX = -1;
    _locusparadcb->offsetY = 0;
    _locusparadcb->pictureNumber = 0;
    _locusparadcb->recycleTotal = (times / 2);

    output->frameTotal = times;
    LBM_FreeLedBitmap(bitmap);
    return result;

}
int DB_CrewelScan(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_CrewelScan_Create(input, output, pparams);
    return DB_CrewelScan_MakeBinaryDetail(input, output, pparams);
}
