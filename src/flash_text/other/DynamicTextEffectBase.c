#include "DynamicTextEffectBase.h"





int DT_Base_ProcessBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                          LedBitmap *bitmap, int x, int y)
{
    int result = -1;
    switch(input->style)
    {
    case SFT_HeartBeat:
        result = DT_HeartBeat(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_Waltz:
        result = DT_Waltz(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_Excursion:
        result = DT_Excursion(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_ExcursionReverse:
        result = DT_ExcursionReverse(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RadioWave:
        result = DT_RadioWave(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_FlashBulb:
        result = DT_FlashBulb(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_Spring:
        result = DT_Spring(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RollOver:
        result = DT_RollOver(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_Domino:
        result = DT_Domino(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_Swirling:
        result = DT_Swirling(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_NeonLight:
        result = DT_NeonLight(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RandomColor:
        result = DT_RandomColor(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RainbowUp:
        result = DT_RainbowUp(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RainbowDown:
        result = DT_RainbowDown(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RainbowLeft:
        result = DT_RainbowLeft(input, output, pparams,
                              bitmap, x, y);
        break;
    case SFT_RainbowRight:
        result = DT_RainbowRight(input, output, pparams,
                              bitmap, x, y);
        break;
    }
    return result;
}
int DT_Base_MakeBinary(const ShineFontAreaInput *input, ShineFontAreaOutput *output)
{
    int result = -1;
    DtElementBaseParams baseParams;
    LBM_PointF startPoint;
    LedBitmap *bitmap;
    int realWidth;
    int x;
    int y;
    char *c = (char *)malloc(strlen(input->text)+1);
    int i;
    int J;
    int N;
    int lastIndex;
    int wordWidth;


    //TBitmap bmp;
    //TLedBitmap tmpBitmap;
    //TGPGraphics Graphics;

    int textCount = getWordCount(input->text);
    KGColor kgcolor;
    LBM_Init();
    TTF_Init();

    baseParams.State = Normal;
    baseParams.AllText = false;
    baseParams.TextCount = textCount;
    baseParams.ColorBitmap = NULL;

    baseParams.Elements.isnull = true;
    baseParams.Elements.next = NULL;
    baseParams.Bitmaps.isnull = true;
    baseParams.Bitmaps.next = NULL;


    bitmap = LBM_DrawFont(input->text,input->fontFile,input->fontSize,KG_COLOR_RED,input->bold,input->italic,input->underline);
    if(bitmap!=NULL)
    {
        //ShineElementTable *_elementInitialInfo;
        //ShineLocusTable *_locusparadcb;
        //_elementInitialInfo = DB_Base_ShineElementTableAdd(&(baseParams.Elements));
        //for(i= 0; i<=50 - 1 - 1; i++)
        //{
        //    _locusparadcb = DB_Base_ShineLocusTableAdd(&(_elementInitialInfo->locusTable));
        //}
        if(input->colorMode==FONT_COLOR_MODE_FILE)
        {
            baseParams.ColorBitmap = SDL_LoadBMP(input->colorFile);
        }

        baseParams.TextWidth = bitmap->w;
        baseParams.TextHeight = bitmap->h;
        //printf("text w: %d\n",baseParams.TextWidth);

        startPoint.X = (input->coord.width-bitmap->w)/2;
        startPoint.Y = (input->coord.height-bitmap->h)/2;
        LBM_FreeLedBitmap(bitmap);


        baseParams.CurrentCharIndex = 0;
        //Attr = TGpImageAttributes.Create;
        //Attr.SetColorKey(TLedColor.TLedColor(TEasyLedColor.TransparentColor).AColor,
        //                 TLedColor.TLedColor(TEasyLedColor.TransparentColor).AColor);

        while(baseParams.CurrentCharIndex < textCount)
        {
            baseParams.CurrentCharIndex++;
            memset(c,0,strlen(input->text)+1);
            if( baseParams.AllText)
            {
//
                //c = Copy(FDynamicText.Text, baseParams.CurrentCharIndex, Length(FDynamicText.Text));
                getAllWordByIndex(input->text,baseParams.CurrentCharIndex,c);
            }
            else
            {
                //c = Copy(FDynamicText.Text, baseParams.CurrentCharIndex, 1);
                getWordByIndex(input->text,baseParams.CurrentCharIndex,c);
            }


            if(input->colorMode==FONT_COLOR_MODE_RANDOM)
            {
                //random
                //bitmap = ClockMaker.DrawText(FDynamicText.FontS, FDynamicText.RandomColors[baseParams.CurrentCharIndex-1].Color, c,nil);
                kgcolor = input->colors[getRandom(input->colorCount)];
            }
            else if(input->colorMode==FONT_COLOR_MODE_FILE)
            {
                //bitmap = ClockMaker.DrawText(FDynamicText.FontS, clWhite, c,nil);
                kgcolor  = KG_COLOR_WHITE;
            }
            else
            {
                //bitmap = ClockMaker.DrawText(FDynamicText.FontS, FDynamicText.Font.Color.Color, c,nil);
                kgcolor = input->colors[0];
            }
            bitmap = LBM_DrawFont(c,input->fontFile,input->fontSize,kgcolor,input->bold,input->italic,input->underline);

            if(input->colorMode==FONT_COLOR_MODE_FILE)
            {
                //DynamicImageMaker.FillCustumColor(bitmap,Rect(0,0,bitmap.Width,bitmap.Height),
                //                                  FDynamicText.Font.Color,not ((FDynamicText.ColorMode==TEasyColorMode.COLORM_SEVEN)
                //                                          and (FDynamicText.ColorGrayed==TEasyColorGrayed.COLORG_256)));
                DB_Base_FillCustumColor(bitmap, bitmap->clip_rect,kgcolor, baseParams.ColorBitmap, input->isColorTile);
            }

            realWidth = bitmap->w;
            x = startPoint.X;
            y = startPoint.Y;


//

            //ProcessBinary(bitmap, x, y);

            result = DT_Base_ProcessBinary(input, output, &baseParams,
                                           bitmap, x, y);


            startPoint.X = startPoint.X + realWidth;
            if( baseParams.AllText)
            {
//
                baseParams.CurrentCharIndex = textCount;
            }
            LBM_FreeLedBitmap(bitmap);
        }


    }

    if(result==0)
    {
        ShineElementTable *pelement;
        ShineLocusTable *plocus;
        ShinePictureTable *pbitmap;
        int j;
        int len = DB_Base_ShineElementTableCount(&(baseParams.Elements));
        //printf("Elements %d: \n",len);

        if(len>0)
        {
            output->elementPtr = (ShineElement *)malloc(len*sizeof(ShineElement));
            for(i=0; i<len; i++)
            {
                pelement = DB_Base_ShineElementTableAt(&(baseParams.Elements),i);
                //printf("\tElement %d: \n",i);
                output->elementPtr[i].locusTotal = DB_Base_ShineLocusTableCount(&(pelement->locusTable));
                //printf("\t\tLocus %d: \n",output->elementPtr[i].locusTotal);
                if(output->elementPtr[i].locusTotal>0)
                {
                    output->elementPtr[i].locusPtr = (ShineLocus *)malloc(output->elementPtr[i].locusTotal*sizeof(ShineLocus));
                    for(j=0; j<output->elementPtr[i].locusTotal; j++)
                    {
                        plocus = DB_Base_ShineLocusTableAt(&(pelement->locusTable),j);
                        output->elementPtr[i].locusPtr[j].nextPic = plocus->nextPic;
                        output->elementPtr[i].locusPtr[j].offsetX = plocus->offsetX;
                        output->elementPtr[i].locusPtr[j].offsetY = plocus->offsetY;
                        output->elementPtr[i].locusPtr[j].pictureNumber = plocus->pictureNumber;
                        output->elementPtr[i].locusPtr[j].recycleTotal = plocus->recycleTotal;
                        //printf("\t\t\tLocus %d: nextPic-%d,offsetX-%d,offsetY-%d,pictureNumber-%d,recycleTotal-%d\n",
                        //       j,output->elementPtr[i].locusPtr[j].nextPic,
                        //       output->elementPtr[i].locusPtr[j].offsetX,
                        //       output->elementPtr[i].locusPtr[j].offsetY,
                        //       output->elementPtr[i].locusPtr[j].pictureNumber,
                        //       output->elementPtr[i].locusPtr[j].recycleTotal);
                    }
                }
                else
                {
                    output->elementPtr[i].locusPtr = NULL;
                }

                //if(!baseParams.IsOverrideFrameCount)
                //{
                //    output->frameTotal = output->elementPtr[i].locusTotal + 1;
                //}
                output->elementPtr[i].startPictureNumber = pelement->startPictureNumber;
                output->elementPtr[i].startX = pelement->startX;
                output->elementPtr[i].startY = pelement->startY;

                //printf("\tElement %d: frameTotal-%d,startPictureNumber-%d,startX-%d,startY-%d\n",i,output->frameTotal,
                //       output->elementPtr[i].startPictureNumber,
                //       output->elementPtr[i].startX,
                //       output->elementPtr[i].startY);

                DB_Base_ShineLocusTableFree(&(pelement->locusTable));
            }
        }
        else
        {
            output->elementPtr = NULL;
        }
        output->elementTotal = len;

        //bitmaps
        len = DB_Base_ShinePictureTableCount(&(baseParams.Bitmaps));

        //printf("Bitmaps %d: \n",len);
        if(len>0)
        {
            output->picturePtr = (ShinePicture *)malloc(len*sizeof(ShinePicture));
            for(i=0; i<len; i++)
            {
                pbitmap = DB_Base_ShinePictureTableAt(&(baseParams.Bitmaps),i);
                output->picturePtr[i].width = pbitmap->width;
                output->picturePtr[i].height = pbitmap->height;
                strcpy(output->picturePtr[i].pictureContent,pbitmap->pictureContent);
                if(input->createMode==KG_CREATE_MEMORY)
                {
                    //output->picturePtr[i].bitmap = pbitmap->bitmap;
                    output->picturePtr[i].bitmap.format = pbitmap->bitmap.format;
                    output->picturePtr[i].bitmap.height = pbitmap->bitmap.height;
                    output->picturePtr[i].bitmap.size = pbitmap->bitmap.size;
                    output->picturePtr[i].bitmap.width = pbitmap->bitmap.width;
                    output->picturePtr[i].bitmap.data = (unsigned char *)malloc(pbitmap->bitmap.size);
                    memcpy(output->picturePtr[i].bitmap.data,pbitmap->bitmap.data,pbitmap->bitmap.size);
                    free(pbitmap->bitmap.data);
                }
                else
                {
                    output->picturePtr[i].bitmap.data = NULL;
                }
                //printf("\tBitmap %d: %s\n",i,output->picturePtr[i].pictureContent);
            }
        }
        else
        {
            output->picturePtr = NULL;
        }
        output->pictureTotal = len;
    }
    DB_Base_ShineElementTableFree(&(baseParams.Elements));
    DB_Base_ShinePictureTableFree(&(baseParams.Bitmaps));
    free(c);
    if(baseParams.ColorBitmap!=NULL)
    {
        LBM_FreeLedBitmap(baseParams.ColorBitmap);
    }
    TTF_Quit();
    LBM_Free();
    return result;
}
int __stdcall shineFontAreaWork(const ShineFontAreaInput *input, ShineFontAreaOutput *output)
{


    char fileSep[2] = {'\0','\0'};
    memset(output,0,sizeof(ShineFontAreaOutput));
    if(strlen(input->dir)==0)
    {
        strcpy(input->dir,"/");
    }
    if(strstr(input->dir,"\\")!=NULL)
    {
        fileSep[0] = '\\';
    }
    else
    {
        fileSep[0] = '/';
    }
    if(input->dir[strlen(input->dir)-1]!=fileSep[0])
    {
        strcat(input->dir,fileSep);
    }
    return DT_Base_MakeBinary(input, output);
}
int __stdcall freeShineFontArea(ShineFontAreaOutput *output)
{
    int i;
    for(i=0; i<output->elementTotal; i++)
    {
        if(output->elementPtr[i].locusPtr!=NULL)
        {
            free(output->elementPtr[i].locusPtr);
        }
    }
    if(output->elementPtr!=NULL)
    {
        free(output->elementPtr);
    }
    if(output->picturePtr!=NULL)
    {
        for(i=0; i<output->pictureTotal; i++)
        {
            if(output->picturePtr[i].bitmap.data!=NULL)
            {
                free(output->picturePtr[i].bitmap.data);
            }
        }
        free(output->picturePtr);
    }
    return 0;
}

