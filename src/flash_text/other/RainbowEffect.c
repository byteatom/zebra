#include "DynamicTextEffectBase.h"


//
// <param name="dynamicTextInfo"></param>
void DT_Rainbow_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{

    pparams->AllText = true;
    //FDirection = TRainbowDirection.rdLeft;
    output->frameTotal = pparams->TextHeight;


}

//
//
//
//
//
LedBitmap* DT_Rainbow_ProcessBitmap(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                                    int index, LedBitmap *bitmap, int *px, int *py,
                                    RainbowDirection FDirection)
{
    int FDistance = output->frameTotal;
    float step;
    LedBitmap *image,*ledcolorbitmap;

    float width;
    float height;


    KGColor ledcolor;
    int start;

    int left;
    int top;

    if(input->colorMode==FONT_COLOR_MODE_FILE)
    {
        char *c = (char *)malloc(strlen(input->text)+1);
        memset(c,0,strlen(input->text)+1);
        ledcolor = input->colors[0];


//
        step = 1.0 * FDistance / output->frameTotal;


        start = ceil(step*(index-1));

        if( (FDirection==rdUp) || (FDirection==rdLeft))
        {
            start = FDistance-start;
        }

        //c = Copy(FDynamicText.Text,FCurrentCharIndex,Length(FDynamicText.Text));
        getAllWordByIndex(input->text,pparams->CurrentCharIndex,c);
        //image = ClockMaker.DrawText(FDynamicText.FontS,  clWhite, c,nil);
        image = LBM_DrawFont(c,input->fontFile,input->fontSize,KG_COLOR_WHITE,input->bold,input->italic,input->underline);

//
//
        ledcolorbitmap = LBM_CreateLedBitmapRGB(image->w,image->h,KG_BMP_32BIT);

        if( (FDirection==rdLeft) || (FDirection==rdRight))
        {
//
//
            left = start;
            while(left>0)
            {
                top = 0;
                while(top<ledcolorbitmap->h)
                {
                    LBM_Draw(ledcolorbitmap,left-pparams->ColorBitmap->w,top,pparams->ColorBitmap);
                    top += pparams->ColorBitmap->h;
                }
                left -= pparams->ColorBitmap->w;
            }
//
            left = start;
            while(left<ledcolorbitmap->w)
            {
                top = 0;
                while(top<ledcolorbitmap->h)
                {
                    LBM_Draw(ledcolorbitmap,left,top,pparams->ColorBitmap);
                    top += pparams->ColorBitmap->h;
                }
                left += pparams->ColorBitmap->w;
            }
        }
        else
        {
//
//
            top = start;
            while(top>0)
            {
                left = 0;
                while(left<ledcolorbitmap->w)
                {
                    LBM_Draw(ledcolorbitmap,left,top-pparams->ColorBitmap->h,pparams->ColorBitmap);
                    left += pparams->ColorBitmap->w;
                }
                top -= pparams->ColorBitmap->h;
            }
//
            top = start;
            while(top<ledcolorbitmap->h)
            {
                left = 0;
                while(left<ledcolorbitmap->w)
                {
                    LBM_Draw(ledcolorbitmap,left,top,pparams->ColorBitmap);
                    left += pparams->ColorBitmap->w;
                }
                top += pparams->ColorBitmap->h;
            }
        }



//



//
        DB_Base_FillCustumColor(image,image->clip_rect,ledcolor,ledcolorbitmap,true);




        free(c);
    }
    else
    {
        image = LBM_Clone(bitmap);
    }
    return image;
}

int DT_Rainbow_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                             LedBitmap *bitmap, int x, int y,RainbowDirection FDirection)
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
        //image := TLedBitmap.Create(realWidth, realHeight);
        //image = TLedBitmap(bitmap.Clone(MakeRect(0,0,bitmap.Width,bitmap.Height),
        //  bitmap.GetPixelFormat));
        /*
            graphics = TGPGraphics.Create(image);

            width = step * i;
            if (i == output->frameTotal)
            {
              width = realWidth;
            }
            graphics.Clear(TColorFilter.Instance.RealTransparentColor.AColor);
            graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
            graphics.DrawImage(bitmap, MakeRect(0, 0, width, realHeight),
              realWidth - width, 0, realWidth, realHeight, TUnit.UnitPixel);
        */

        image = DT_Rainbow_ProcessBitmap(input,output,pparams,i,bitmap,&x,&y,FDirection);

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



//

    _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - output->frameTotal);
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;
    return result;
}
