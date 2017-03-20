#include <stdlib.h>
#include "Common.h"
#include "DynamicBackgroundEffectBase.h"





//
//
//
/*
constructor DynamicBackgroundEffectBase.Create(_programInfo: ProgramInfo;_bounds: TLedRect);
{
inherited Create();
        GroupCount = 1;
        CurrentLoopIndex = 0;
        FProgramInfo = _programInfo;
        FBounds = _bounds;
//
        ShapeCount = RealToInt(Math.Ceil((_bounds.Width * _bounds.Height) / (1024.0 * 4)));
        LoopCount = ShapeCount * 4;
        BitmapInfoDictionaries = TMapIntInt.Create();
}
destructor DynamicBackgroundEffectBase.Destroy();
{
  FreeAndNil(BitmapInfoDictionaries);
  if( ElementInitialInfos!=nil)
  {
    FreeAndNil(ElementInitialInfos);
  }
  inherited Destroy;
}
*/


//
//
//

int DB_Base_AddPlaceHolderBitmap(ShinePictureTable *bitmaps,const char *dir,KGPicType pictype,KGCreateMode createMode)
{
    int result;
    LedBitmap *bitmap;

//
    bitmap = LBM_CreateLedBitmap(8, 8, pictype);
    //graphics = TGPGraphics.Create(bitmap);

    //graphics.Clear(TColorFilter.Instance.RealTransparentColor.AColor);


    result = DB_Base_BoxingBitmap(bitmaps,dir,bitmap,createMode);

    LBM_FreeLedBitmap(bitmap);
    return result;
}


/*
TLedBitmap DB_Base_GetBitmap()
{
        raise Exception.Create('NotImplemented');
}
TLedBitmap DB_Base_GetFrameBitmap(int index, int varframeCount)
{
    TLedBitmap bitmap;
    TGPGraphics graphics;
    TGPSolidBrush sb;
        //Console.WriteLine(index);
        frameCount = FBounds.Width - 1;
        FrameCount = FBounds.Width - 1;
        bitmap = TLedBitmap.Create(FBounds.Width, FBounds.Height);
        graphics = TGPGraphics.Create(bitmap);
        sb = TGPSolidBrush.Create(TLedColor.TLedColor(clRed).AColor);
        graphics.FillRectangle(sb, index, 0, 8, 8);
        FreeAndNil(sb);
        FreeAndNil(graphics);
        Result = bitmap;
        return;
}
*/
int DB_Base_MakeBinary(const BackgroundInput *input, BackgroundOutput *output)
{
    int result = -1;
    BgElementBaseParams baseParams;
    ShineElementTable *pelement;
    ShineLocusTable *plocus;
    ShinePictureTable *pbitmap;

    int len;
    int i;
    int j;

    LBM_Init();

    baseParams.GroupCount = 1;
    baseParams.CurrentLoopIndex = 0;
    baseParams.IsOverrideFrameCount = false;

    baseParams.BitmapInfoDictionaries.isnull = true;
    baseParams.BitmapInfoDictionaries.next = NULL;
    baseParams.Elements.isnull = true;
    baseParams.Elements.next = NULL;
    baseParams.Bitmaps.isnull = true;
    baseParams.Bitmaps.next = NULL;

//
    baseParams.ShapeCount = (ceil((input->coord.width * input->coord.height) / (1024.0 * 4)));
    baseParams.LoopCount = baseParams.ShapeCount * 4;

    len = baseParams.ShapeCount*baseParams.GroupCount;
    //ElementInitialInfos = TListElementInitialInfo.Create();

    //for(i=0; i<=len-1; i++)
    //{
    //    ElementInitialInfos.Add(nil);
    //}
    switch(input->style)
    {
    case BGS_Cirque:
    {
        result = DB_Cirque(input,output,&baseParams);
        break;
    }
    case BGS_FivePointedStar:
    {
        result = DB_FivePointedStar(input,output,&baseParams);
        break;
    }
    case BGS_Babysbreath:
    {
        result = DB_Babysbreath(input,output,&baseParams);
        break;
    }
    case BGS_Snow:
    {
        result = DB_Snow(input,output,&baseParams);
        break;
    }
    case BGS_Arrow:
    {
        result = DB_Arrow(input,output,&baseParams);
        break;
    }
    case BGS_SideswaySlipLine:
    {
        result = DB_SideswaySlipLine(input,output,&baseParams);
        break;
    }
    case BGS_FaceToFaceSlipLine:
    {
        result = DB_FaceToFaceSlipLine(input,output,&baseParams);
        break;
    }
    case BGS_Diamond:
    {
        result = DB_Diamond(input,output,&baseParams);
        break;
    }
    case BGS_Block:
    {
        result = DB_Block(input,output,&baseParams);
        break;
    }
    case BGS_Circle:
    {
        result = DB_Circle(input,output,&baseParams);
        break;
    }
    case BGS_CrewelScan:
    {
        result = DB_CrewelScan(input,output,&baseParams);
        break;
    }
    case BGS_SingleScan:
    {
        result = DB_SingleScan(input,output,&baseParams);
        break;
    }

    }

    if(result==0)
    {
        len = DB_Base_ShineElementTableCount(&(baseParams.Elements));
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

                if(!baseParams.IsOverrideFrameCount)
                {
                    output->frameTotal = output->elementPtr[i].locusTotal + 1;
                }
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
                    output->picturePtr[i].bitmap = pbitmap->bitmap;
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

    LBM_Free();
    return result;


}
int __stdcall backgroundWork(const BackgroundInput *input, BackgroundOutput *output)
{
    char fileSep[2] = {'\0','\0'};
    memset(output,0,sizeof(BackgroundOutput));
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
    return DB_Base_MakeBinary(input,output);
}
int __stdcall freeBackground(BackgroundOutput *output)
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
        for(i=0;i<output->pictureTotal;i++)
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
