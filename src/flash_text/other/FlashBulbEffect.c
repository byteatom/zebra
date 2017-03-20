#include "DynamicTextEffectBase.h"

const int DT_FlashBulb_singleCharFrameCount = 6;
//
// <param name="_dynamicTextInfo"></param>
void DT_FlashBulb_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{


    output->frameTotal = DT_FlashBulb_singleCharFrameCount * pparams->TextCount;
}

int DT_FlashBulb_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                               LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    KGColor color;
    int size;
    int centerX;
    int startY;
    LedBitmap *image;
    int i;
    int step;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;
    //???
    //image = LBM_CreateLedBitmap(8,8,KG_BMP_32BIT);
    //color := TLedColor.TLedColor(FDynamicText.Font.Color.Color);
    if(input->colorMode==FONT_COLOR_MODE_RANDOM)
    {
//
        color = input->colors[pparams->CurrentCharIndex-1];
    }
    else
    {
        color = input->colors[0];//color = TLedColor.TLedColor(FDynamicText.Font.Color.Color);
    }
    //if(Bitmaps.Count == 0)
    //{
//
    //    Bitmap allLightImage = new Bitmap(Bounds.Width, Bounds.Height);
    //    using (Graphics graphics = TGPGraphics.Create(allLightImage))
    //    {
    //        graphics.Clear(color);
//    */
    //    Bitmaps.Add(allLightImage);
    //    Icoparadcbs.Add(new ICOPARADCB { Width = (Int16)allLightImage.Width, Height = (Int16)allLightImage.Height });
    //    Bitmap allDarkImage = (Bitmap)allLightImage.Clone();
    //    allDarkImage.MakeTransparent(color);
    //    Bitmaps.Add(allDarkImage);
    //    Icoparadcbs.Add(new ICOPARADCB { Width = (Int16)allDarkImage.Width, Height = (Int16)allDarkImage.Height });
//*/
    size = min(bitmap->w, bitmap->h);
    centerX = bitmap->w / 2;
    startY = (bitmap->h - size) / 2;
    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
//
    for(i=1; i<=DT_FlashBulb_singleCharFrameCount; i++)
    {

        step = i % DT_FlashBulb_singleCharFrameCount;
        switch (step)
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

            thickLineColor(image, centerX, 0, centerX, bitmap->h,2,color);
            break;
        }

        case 4:
        {
//
            image = LBM_CreateLedBitmap(input->coord.width, input->coord.height,KG_BMP_32BIT);
            LBM_FillLedBitmap(image,color);
            break;
        }

        case 5:
        {
//
            image = LBM_CreateLedBitmap(input->coord.width, input->coord.height,KG_BMP_32BIT);
            //graphics.Clear(TLedColor.TLedColor(clYellow).AColor);
            LBM_FillLedBitmap(image,KG_COLOR_YELLOW);
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
        if( (((pparams->CurrentCharIndex == 1)  &&  (i > 1))  ||  (pparams->CurrentCharIndex > 1)))
        {
//
//
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),((pparams->CurrentCharIndex-1)*DT_FlashBulb_singleCharFrameCount)+(i-2));
            _locusparadcb->pictureNumber = ((pparams->CurrentCharIndex - 1) * DT_FlashBulb_singleCharFrameCount + (i - 1));
            _locusparadcb->nextPic = 1;
            if (step == 4)
            {
                _locusparadcb->offsetX = (-x);
                _locusparadcb->offsetY = (-y);
            }
            else if(step == 5)
            {
                _locusparadcb->offsetX = 0;
                _locusparadcb->offsetY = 0;
            }
            else if(step == 0)
            {
                _locusparadcb->offsetX = (x);
                _locusparadcb->offsetY = (y);
            }
            //locusparadcbs[((pparams->CurrentCharIndex-1)*DT_FlashBulb_singleCharFrameCount)+(i-2)] = _locusparadcb;
        }
        LBM_FreeLedBitmap(image);
    }

//
    for(i=0; i<=output->frameTotal - DT_FlashBulb_singleCharFrameCount * pparams->CurrentCharIndex-1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),((pparams->CurrentCharIndex-1)*DT_FlashBulb_singleCharFrameCount)+(i+(DT_FlashBulb_singleCharFrameCount-1)));
        //_locusparadcb.PNum = (Int16)(pparams->CurrentCharIndex * DT_FlashBulb_singleCharFrameCount - 1);
        _locusparadcb->pictureNumber = 0;
        _locusparadcb->nextPic = 0;
        //locusparadcbs[((pparams->CurrentCharIndex-1)*DT_FlashBulb_singleCharFrameCount)+(i+(DT_FlashBulb_singleCharFrameCount-1))] = _locusparadcb;
    }
//
    if (pparams->CurrentCharIndex > 1)
    {
        for(i=0; i<=output->frameTotal - (output->frameTotal / DT_FlashBulb_singleCharFrameCount - (pparams->CurrentCharIndex - 1)) * DT_FlashBulb_singleCharFrameCount - 1-1; i++)
        {
            _locusparadcb = DB_Base_ShineLocusTableAt(&(_elementInitialInfo->locusTable),i);
            _locusparadcb->pictureNumber = -1;
            _locusparadcb->nextPic = 0;
            //locusparadcbs[i] = _locusparadcb;
        }
    }
//

//
    if (pparams->CurrentCharIndex == 1)
    {
        _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - DT_FlashBulb_singleCharFrameCount);
    }
    else
    {
        _elementInitialInfo->startPictureNumber = -1;
    }
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
int DT_FlashBulb(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                 LedBitmap *bitmap, int x, int y)
{
    DT_FlashBulb_Create(input, output, pparams);
    return DT_FlashBulb_ProcessBinary(input, output, pparams,bitmap,x,y);
}
