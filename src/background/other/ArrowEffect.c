#include "LedBitmap.h"
#include "DynamicBackgroundEffectBase.h"
#include "Common.h"
#include "MapIntInt.h"

//
//
//
void DB_Arrow_Create(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{

    pparams->ShapeCount = 2;
    pparams->IsOverrideFrameCount = true;
}

//
int DB_Arrow_MakeBinaryDetail(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    int result = -1;
    int penWidth;
    int centerX;
    int centerY;


    int width;
    int height;
    LedBitmap *rightArrowBitmap;

    LedBitmap *leftArrowBitmap;
    int i;
    ShineElementTable *_elementInitialInfo;

    int offsetX;
    ShineLocusTable *_locusparadcb;
//
    if (input->coord.width > input->coord.height)
    {
        penWidth = 3;
        centerX = input->coord.width / 2;
        centerY = input->coord.height / 2;
//
        //graphicsPath = TGPGraphicsPath.Create(FillModeWinding);
        //graphicsPath.AddLine(MakePoint(0, 0), MakePoint(centerY, centerY));
        //graphicsPath.AddLine(MakePoint(centerY, centerY),
        //MakePoint(0, input->coord.height));

        width = ceil(centerX);
        height = input->coord.height;
//
        rightArrowBitmap = LBM_CreateLedBitmap(width + penWidth, height,input->drawPicType);

        thickLineColor(rightArrowBitmap,0, 0,centerY, centerY,penWidth,LBM_KGColorToSdlColor(KG_COLOR_RED));
        thickLineColor(rightArrowBitmap,centerY, centerY,0, input->coord.height,penWidth,LBM_KGColorToSdlColor(KG_COLOR_RED));

//
        leftArrowBitmap = LBM_RotateFlip(rightArrowBitmap,Rotate180FlipY);
//
        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,leftArrowBitmap,input->createMode);
        result = DB_Base_BoxingBitmap(&(pparams->Bitmaps),input->dir,rightArrowBitmap,input->createMode);

        LBM_FreeLedBitmap(rightArrowBitmap);
        LBM_FreeLedBitmap(leftArrowBitmap);

        for(i=0; i<=pparams->ShapeCount - 1; i++)
        {
            _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));

            if (i == 0)
            {
                _elementInitialInfo->startX = (centerX - width - penWidth);
                _elementInitialInfo->startY = 0;
                offsetX = -1;
            }
            else
            {
                _elementInitialInfo->startX = centerX;
                _elementInitialInfo->startY = 0;
                offsetX = 1;
            }

//
//
            //_locusparadcb = LOCUSPARADCB.LOCUSPARADCB(0);
            //_locusparadcb.PNum = 0;
            //_locusparadcb.BNext = 0;
            //_locusparadcb.XOffset = offsetX;
            //_locusparadcb.Recycle = input->coord.width - (centerX + width + penWidth);
            //_elementInitialInfo.Locusparadcbs.Add(_locusparadcb);

            _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
            _locusparadcb->nextPic = 0;
            _locusparadcb->offsetX = offsetX;
            _locusparadcb->offsetY = 0;
            _locusparadcb->pictureNumber = 0;
            _locusparadcb->recycleTotal = input->coord.width - (centerX + width + penWidth);

            output->frameTotal = _locusparadcb->recycleTotal + 1;
        }

    }
    return result;
}
int DB_Arrow(const BackgroundInput *input, BackgroundOutput *output, BgElementBaseParams *pparams)
{
    DB_Arrow_Create(input, output, pparams);
    return DB_Arrow_MakeBinaryDetail(input, output, pparams);
}
