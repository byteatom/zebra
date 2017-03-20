#include "DynamicTextEffectBase.h"


//
// <param name="dynamicTextInfo"></param>
void DT_NeonLight_Create(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams)
{
    int I;
    int J;
    bool bfind;
    Uint32 color;
    /*
//
//
    if (FDynamicText.ColorMode==COLORM_ONE)
    {
//
    SetLength(FColors,1);
    for(I= 0;I<=Length(FColors) - 1;I++)
    {
      FColors[I] = TEasyLedColor.BaseColorOnes[I];
    }
    }
    else if FDynamicText.ColorMode==COLORM_TWO
    {
//
    SetLength(FColors,3);
    for(I= 0;I<=Length(FColors) - 1;I++)
    {
      FColors[I] = TEasyLedColor.BaseColorTwos[I];
    }
    }
    else
    {
    if( FDynamicText.ColorGrayed==COLORG_NONE)
    {
//
      SetLength(FColors,7);
      for(I= 0;I<=Length(FColors) - 1;I++)
      {
        FColors[I] = TEasyLedColor.BaseColorSevens[I];
      }
    }
    else
    {
//
      RandSeed = GetTickCount;
      Randomize();
      SetLength(FColors,7);
      for(I= 0;I<=6;I++)
      {

        while(True)
        {
          bfind = False;
          color = TEasyLedColor.BaseColorAlls[Random(Length(TEasyLedColor.BaseColorAlls))];
          for(J= 0;J<=I-1;J++)
          {
            if( color==FColors[J])
            {
              bfind = True;
              Break;
            }

          }
          if( not bfind)
          {
            Break;
          }
        }
        FColors[I] = color;
      }
    }
    }
    */
    output->frameTotal = 10;

    //FIsRandomColor = False;
    //FStartColorIndex = 0;
}
int DT_NeonLight_GetColorIndex(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                               int index,bool FIsRandomColor)
{
    int colorindex;
    if( FIsRandomColor)
    {
//
        colorindex = getRandom(input->colorCount);
    }
    else
    {
        //if FCurrentCharIndex=1 then
        //begin
        //  FStartColorIndex := index-1;
        //end;
        colorindex = index-1+(pparams->CurrentCharIndex-1);
        if (colorindex>=input->colorCount)
        {
            colorindex = colorindex % input->colorCount;
        }
    }
    //colorindex := Random(Length(FColors));
    return  colorindex;
}



int DT_NeonLight_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                               LedBitmap *bitmap, int x, int y,bool FReverse,bool FIsRandomColor)
{
    int result = -1;
    int FStartColorIndex = 0;
    int i;
    int realWidth;
    int realHeight;
    float step;
    LedBitmap *image;

    float width;
    ShineElementTable *_elementInitialInfo;
    ShineLocusTable *_locusparadcb;
    int colorindex;
    char *c = (char *)malloc(strlen(input->text)+1);
    _elementInitialInfo = DB_Base_ShineElementTableAdd(&(pparams->Elements));
    for(i= 0; i<=output->frameTotal - 1 - 1; i++)
    {
        _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
    }
    for(i= 1; i<=output->frameTotal; i++)
    {
        memset(c,0,strlen(input->text)+1);
        colorindex = DT_NeonLight_GetColorIndex(input,output,pparams,i,FIsRandomColor);
        //c = Copy(FDynamicText.Text,FCurrentCharIndex,1);
        getWordByIndex(input->text,pparams->CurrentCharIndex,c);
        //image = ClockMaker.DrawText(FDynamicText.FontS,  FColors[colorindex], c,nil);
        image = LBM_DrawFont(c,input->fontFile,input->fontSize,input->colors[colorindex],input->bold,input->italic,input->underline);
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
    // tmplocusparadcbs := TListLOCUSPARADCB.Create();
    // Bitmaps.Reverse;
    if( FReverse)
    {
//
        //locusparadcbs.Reverse();
    }



//

    _elementInitialInfo->startPictureNumber = (DB_Base_ShinePictureTableCount(&(pparams->Bitmaps)) - output->frameTotal);
    _elementInitialInfo->startX = x;
    _elementInitialInfo->startY = y;

    free(c);
    return result;
}
int DT_NeonLight(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                 LedBitmap *bitmap, int x, int y)
{
    DT_NeonLight_Create(input, output, pparams);
    return DT_NeonLight_ProcessBinary(input, output, pparams,bitmap,x,y,false,false);
}

