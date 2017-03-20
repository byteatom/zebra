#include "KGWatchFuncs.h"
#include "Common.h"


//
void getFileNamePrefix(LedFileType fileType,const char *dir,long curtime,char *filename)
{
    char fileSep[2] = "\0";
    char flag[20];
    char temp[1024] = "\0";
    if(strstr(dir,"\\")!=NULL)
    {
        fileSep[0] = '\\';
    }
    else
    {
        fileSep[0] = '/';
    }
    fileSep[1] = '\0';
    strcpy(temp,dir);
    if(strlen(temp)>0)
    {
        if(temp[strlen(temp)-1]!=fileSep[0])
        {
            strcat(temp,fileSep);
        }
    }
    switch(fileType)
    {
    case LFT_Dial:
    {
        sprintf(flag,"%s_%u_","dial",curtime);
    }
    break;
    case LFT_HandHour:
    {
        sprintf(flag,"%s_%u_","hour",curtime);
    }
    break;
    case LFT_HandMinute:
    {
        sprintf(flag,"%s_%u_","min",curtime);
    }
    break;
    case LFT_HandSecond:
    {
        sprintf(flag,"%s_%u_","sec",curtime);
    }
    break;
    }
    strcpy(filename,temp);
    strcat(filename,flag);
}
void getFileName(const char *filenameprefix,int index,char *filename)
{
    char temp[1024] = "";
    sprintf(temp,"%s%03d%s",filenameprefix,index,".bmp");
    strcpy(filename,temp);
}
int drawDialPic(const WatchAreaInput *input, WatchAreaOutput *output, const char *path, KGPoint *centre, const DrawDialParaExt *dialParaExt)
{
    LedBitmap *bitmap;
    Sint32 result = -1;

    //LBM_Init();
    bitmap = LBM_CreateLedBitmap(input->dialPara.size.width,input->dialPara.size.height,input->dialPara.drawPicType);
    if(bitmap!=NULL)
    {
        //float ang = 0*PI/180;
        //float x;
        //float y;


        //x = dialParamExt._centerX+(dialParamExt._radius - 4)*cosf(ang);
        //y = dialParamExt._centerY+(dialParamExt._radius - 4)*sinf(ang);

        //filledEllipseColor(bitmap, x, y, 8/2, 8/2, 0xFF0000FF);


        KGW_DrawThicks(bitmap, &(input->dialPara), dialParaExt);
        output->bitmap.data = NULL;
        result = LBM_SaveLedBitmapToFile(bitmap,path,&(output->bitmap),input->createMode);
    }
    LBM_FreeLedBitmap(bitmap);
    //LBM_Free();
    return result;
}

int __stdcall watchAreaWork(const WatchAreaInput *input, WatchAreaOutput *output)
{
    int result = -1;
    int i;
    char filename[1024] = "";
    long curtime = getCurrentTime();
    DrawDialParaExt dialParamExt;
    if(input==NULL || output==NULL)
    {
        return result;
    }
    memset(output,0,sizeof(WatchAreaOutput));
    if(strlen(input->dir)==0)
    {
        strcpy(input->dir,"/");
    }
    LBM_Init();
    dialParamExt._radius = min(input->dialPara.size.width,input->dialPara.size.height)/2;
    dialParamExt._radiusA = dialParamExt._radius;
    if(min(input->dialPara.size.width,input->dialPara.size.height)%2==0)
    {
        dialParamExt._radiusA--;
    }
    dialParamExt._centerX = input->dialPara.size.width / 2;
    dialParamExt._centerY = input->dialPara.size.height / 2;
//
    getFileNamePrefix(LFT_Dial,input->dir,curtime,filename);
    getFileName(filename,0,filename);
    strcpy(output->dialContent,filename);
    result = drawDialPic(input,output, filename, NULL,&dialParamExt);

    if(result==0)
    {
//
        getFileNamePrefix(LFT_HandHour,input->dir,curtime,filename);
        result = KGW_MakeHours(&(input->handPara[0]), &(input->dialPara), &dialParamExt, filename,output->hourHand,input->createMode);

    }
    if(result==0)
    {
//
        getFileNamePrefix(LFT_HandMinute,input->dir,curtime,filename);
        result = KGW_MakeMinutes(&(input->handPara[1]), &(input->dialPara), &dialParamExt, filename,output->minHand,input->createMode);

    }
    if(result==0)
    {
//
        getFileNamePrefix(LFT_HandSecond,input->dir,curtime,filename);
        result = KGW_MakeSeconds(&(input->handPara[2]), &(input->dialPara), &dialParamExt, filename,output->secHand,input->createMode);

    }
    for(i=0; i<4; i++)
    {
        output->jetLag[i] = input->jetLag[i];
    }
    LBM_Free();
    //printf("Used time: %d\n",getCurrentTime()-curtime);
    return result;
}
int __stdcall freeWatchArea(WatchAreaOutput *output)
{
    int i;
    for(i=0;i<60;i++)
    {
        if(output->hourHand[i].bitmap.data!=NULL)
        {
            free(output->hourHand[i].bitmap.data);
        }
        if(output->minHand[i].bitmap.data!=NULL)
        {
            free(output->minHand[i].bitmap.data);
        }
        if(output->secHand[i].bitmap.data!=NULL)
        {
            free(output->secHand[i].bitmap.data);
        }
    }
    if(output->bitmap.data!=NULL)
    {
        free(output->bitmap.data);
    }
    return 0;
}
/// <summary>
//
/// </summary>
//
//
//
//
//
//
//
void KGW_GetPolygon(float centerX, float centerY, float thickness, float length, Uint32 color, float radians, LBM_PointF points[])
{

    points[0] = LBM_MakePointF((centerX + thickness * 2 * sin(radians + PI / 2)),
                               (centerY - thickness * 2 * cos(radians + PI / 2)));

    points[2] = LBM_MakePointF((centerX + thickness * 2 * sin(radians - PI / 2)),
                               (centerY - thickness * 2 * cos(radians - PI / 2)));

    points[3] = LBM_MakePointF((centerX + length * sin(radians)),
                               (centerY - length * cos(radians)));

    points[1] = LBM_MakePointF((centerX - thickness * 4 * sin(radians)),
                               (centerY + thickness * 4 * cos(radians)));

}

/// <summary>
//
/// </summary>
//
//
//
//
//
RECTANGULARPARADCB KGW_GetPolygonRectangle(float centerX, float centerY, float thickness, float length, Uint32 color, float radians)
{
    Sint32 width;
    Sint32 height;
    LBM_PointF points[4];
    LBM_PointF a;
    LBM_PointF d;
    LBM_PointF b;
    LBM_PointF c;
    short left;
    short right;
    short top;
    short bottom;
    RECTANGULARPARADCB Result;
    KGW_GetPolygon(centerX, centerY,thickness, length, color, radians,points);
    a = points[0];
    d = points[1];
    b = points[2];
    c = points[3];

//
    left = round(min(d.X, min(c.X, min(a.X, b.X))));
    right = round(max(d.X, max(c.X, max(a.X, b.X))));
    top = round(min(d.Y, min(c.Y, min(a.Y, b.Y))));
    bottom = round(max(d.Y, max(c.Y, max(a.Y, b.Y))));

    width = right - left;
    height = bottom - top;

    Result.X = left;
    Result.Y = top;
    Result.Width = (right - left)+1;
    Result.Height = (bottom - top)+1;
    return Result;

}



/// <summary>
//
/// </summary>
//
//
//
//
//
//
//
void KGW_DrawPolygon(HandType handType, float centerX, float centerY, float thickness, float slength, Uint32 color, float radians, LedBitmap *bitmap, LBM_Point *pLeft)
{
    LBM_PointF points[4];
    Sint16 vx[4],vy[4];
    int i;
    KGW_GetPolygon(centerX, centerY, thickness, slength, color, radians,points);
    for(i=0; i<4; i++)
    {
        vx[i] = points[i].X;
        vy[i] = points[i].Y;
        if(pLeft!=NULL)
        {
            vx[i] -= pLeft->X;
            vy[i] -= pLeft->Y;
        }
    }

    if(handType == HAND_RECTANGLE)
    {
        //Graphics.DrawLine(pen,points[1],points[3])
        thickLineColor(bitmap,vx[1],vy[1],vx[3],vy[3],thickness,color);
        //thickLineRGBA(bitmap,points[1].X,points[1].Y,points[3].X,points[3].Y,thickness,0xFF,0x00,0x00,0xFF);
    }
    else
    {

        filledPolygonColor(bitmap, &vx[0], &vy[0], 4, color);
        lineColor(bitmap,vx[0],vy[0],vx[1],vy[1],color);
        lineColor(bitmap,vx[1],vy[1],vx[2],vy[2],color);
        lineColor(bitmap,vx[2],vy[2],vx[3],vy[3],color);
        lineColor(bitmap,vx[3],vy[3],vx[0],vy[0],color);
        lineColor(bitmap,vx[0],vy[0],vx[2],vy[2],color);
        //lineColor(bitmap,vx[1],vy[1],vx[3],vy[3],color);
        //thickLineColor(bitmap,vx[1],vy[1],vx[3],vy[3],2,color);
        /*
        filledPolygonRGBA(bitmap, &vx[0], &vy[0], 4, 0xFF,0x00,0x00,0xFF);
        lineRGBA(bitmap,points[0].X,points[0].Y,points[1].X,points[1].Y,0xFF,0x00,0x00,0xFF);
        lineRGBA(bitmap,points[1].X,points[1].Y,points[2].X,points[2].Y,0xFF,0x00,0x00,0xFF);
        lineRGBA(bitmap,points[2].X,points[2].Y,points[3].X,points[3].Y,0xFF,0x00,0x00,0xFF);
        lineRGBA(bitmap,points[3].X,points[3].Y,points[0].X,points[0].Y,0xFF,0x00,0x00,0xFF);
        */
    }




}
/*
/// <summary>
//
/// </summary>
//
//
//
//
//
void KGW_Make(TBitmap Graphics, Sint32 Hour, Sint32 Minute, Sint32 Second)
{
    float hourRadians; float minuteRadians; float secondRadians;
    TGPGraphics GPGraphics;

  GPGraphics = TGPGraphics.Create(Graphics.Canvas.Handle);
    if( (_width <= 0) || (_height <= 0))
    {
      return;
    }

    Graphics.width = _width;
    Graphics.height = _height;

//
//
    if( drawPara->dialType == TWatchStyle.WS_NORMAL)
    {
      MakeThicks(GPGraphics);
      if( drawPara->IsShowDigital)
      {
        MakeDigitals(GPGraphics);
      }
    }
//
    if( drawPara->dialType == TWatchStyle.WS_QUADRANT)
    {
      MakeRoundRectThicks(GPGraphics);
      if( drawPara->IsShowDigital)
      {
//
        MakeRoundRectDigitals(GPGraphics);
      }
    }
//
    if( drawPara->dialType == TWatchStyle.WS_ELLIPSE)
    {
      MakeEllipseThicks(GPGraphics);
      if( drawPara->IsShowDigital)
      {
//
        MakeEllipseDigitals(GPGraphics);
      }
    }
//
    if( drawPara->dialType == TWatchStyle.WS_RHOMBUS)
    {
      MakeRhombusThicks(GPGraphics);
      if( drawPara->IsShowDigital)
      {
//
        MakeRhombusDigitals(GPGraphics);
      }
    }
//
    if( drawPara->dialType == TWatchStyle.WS_WINDMILL)
    {
      MakePolygon12Thicks(GPGraphics);
      if( drawPara->IsShowDigital)
      {
//
        MakePolygon12Digitals(GPGraphics);
      }
    }

    if (drawPara->DrawHourHand.width > 0)
    {
      hourRadians = (Hour % 12 + Minute / 60) * 30 * PI / 180;

//
      DrawPolygon(drawPara->DrawHourHand.width, _hourLength,
        drawPara->DrawHourHand.color, hourRadians, GPGraphics);
    }

    if (drawPara->DrawMinuteHand.width > 0)
    {
      minuteRadians = Minute * 6 * PI / 180;

//
      DrawPolygon(drawPara->DrawMinuteHand.width, _minuteLength,
        drawPara->DrawMinuteHand.color, minuteRadians, GPGraphics);
    }

    if (drawPara->DrawSecondHand.width > 0)
    {

      secondRadians = Second * 6 * PI / 180;

//
      DrawPolygon(drawPara->DrawSecondHand.Width, _secondLength, drawPara->DrawSecondHand.color,
        secondRadians, GPGraphics);
//      DrawPolygon(1, _secondLength, drawPara->DrawSecondHand.color,
//        secondRadians, GPGraphics);
    }

}
*/
int KGW_MakeHours(const DrawHandPara *handPara, const DrawDialPara *dialPara, const DrawDialParaExt *dialParaExt,
                  const char *fileNamePrefix, WatchHand watchHands[],KGCreateMode createMode)
{
    int result = -1;
    char filename[1024],bmpfile[1024];
    Sint32 capacity;
    LedBitmap *bitmap,*backgroundBitmap,*bmp0,*bmp;
    int index = 0;
    Sint32 i;
    Sint32 j;
    float hourRadians;
    Sint32 hourHandRadius;
    Uint32 hourHandColor;
    RECTANGULARPARADCB _rectangularparadcb,rcbitmap;

    Sint32 _hourLength = handPara->size.height;

    LBM_Point pLeft;
    int bw,bh;

    capacity = 60;

    backgroundBitmap = LBM_CreateLedBitmap(dialPara->size.width, dialPara->size.height,handPara->drawPicType);
//
    //for(i= 0; i<=12 - 1; i++)
    //{
    // for j := 0 to 60; j+=12) do
    //    j = 0;
    //     while(j < 60)
    //     {
    for(i= 0; i<=capacity - 1; i++)
    {

        //hourRadians = (i + j / 60) * 30 * PI / 180;
        hourRadians = i * 6 * PI / 180;
//
        hourHandRadius = handPara->size.width; // .HourHandRadius;
        hourHandColor = LBM_KGColorToSdlColor(handPara->color);
        // .HourHandColor);
        if (hourHandRadius == 0)
        {
            hourHandRadius = 1;
            hourHandColor = LBM_KGColorToSdlColor(KG_COLOR_BLACK);

        }
        _rectangularparadcb = KGW_GetPolygonRectangle(dialParaExt->_centerX,dialParaExt->_centerY, hourHandRadius, _hourLength,
                              hourHandColor, hourRadians);


        //LBM_ClearLedBitmap(backgroundBitmap);
//
        //KGW_DrawPolygon(handPara->handType, dialParaExt->_centerX,dialParaExt->_centerY,hourHandRadius, _hourLength, hourHandColor, hourRadians,
        //                backgroundBitmap);

//
        bitmap = LBM_CreateLedBitmap(_rectangularparadcb.Width,
                                     _rectangularparadcb.Height,handPara->drawPicType);

//
        //Graphics.DrawImage(backgroundBitmap, MakeRect(0, 0, bitmap.width,
        //                   bitmap.height), _rectangularparadcb.X, _rectangularparadcb.Y,
        //                   _rectangularparadcb.width, _rectangularparadcb.height, TUnit.UnitPixel);
        rcbitmap.X = 0;
        rcbitmap.Y = 0;
        rcbitmap.Width = bitmap->w;
        rcbitmap.Height = bitmap->h;
        //LBM_CopyLebBitmap(backgroundBitmap,_rectangularparadcb,bitmap,rcbitmap);

        pLeft = LBM_MakePoint(_rectangularparadcb.X,_rectangularparadcb.Y);
        KGW_DrawPolygon(handPara->handType, dialParaExt->_centerX,dialParaExt->_centerY,hourHandRadius, _hourLength, hourHandColor, hourRadians,
                        bitmap,&pLeft);



        getFileName(fileNamePrefix,index,filename);
        if(i==0)
        {
            bmp0 = LBM_Clone(bitmap);
        }
        bw = bitmap->w;
        bh = bitmap->h;
        LBM_FreeLedBitmap(bitmap);
        bmp = rotozoomSurface(bmp0,-i*6,1,0);
        bitmap = LBM_CreateLedBitmap(bw,bh,KG_BMP_32BIT);
        LBM_Draw(bitmap,(bitmap->w-bmp->w)/2,(bitmap->h-bmp->h)/2,bmp);
        LBM_FreeLedBitmap(bmp);
        //printf("%d,%d; %d,%d\n",bw,bh,bitmap->w,bitmap->h);


        result = LBM_SaveLedBitmapToFile(bitmap,filename,&(watchHands[index].bitmap),createMode);

        watchHands[index].coord.x = _rectangularparadcb.X;
        watchHands[index].coord.y = _rectangularparadcb.Y;
        watchHands[index].coord.width = _rectangularparadcb.Width;
        watchHands[index].coord.height = _rectangularparadcb.Height;
        strcpy(watchHands[index].handContent,filename);




        index++;


        LBM_FreeLedBitmap(bitmap);
        if(result!=0)
        {
            break;
        }
        //j += 12;
        //}
    }
    LBM_FreeLedBitmap(backgroundBitmap);
    LBM_FreeLedBitmap(bmp0);
    return result;
}

int KGW_MakeMinutes(const DrawHandPara *handPara, const DrawDialPara *dialPara, const DrawDialParaExt *dialParaExt,
                    const char *fileNamePrefix, WatchHand watchHands[],KGCreateMode createMode)
{
    int result = -1;
    char filename[1024];

    LedBitmap *bitmap,*backgroundBitmap,*bmp0,*bmp;
    int bw,bh;
    int index = 0;
    Sint32 capacity;

    Sint32 i;
    float minuteRadians;
    Sint32 minuteHandRadius;
    Uint32 minuteHandColor;
    RECTANGULARPARADCB _rectangularparadcb,rcbitmap;
    Sint32 _minuteLength = handPara->size.height;
    LBM_Point pLeft;
    capacity = 60;
//

    backgroundBitmap = LBM_CreateLedBitmap(dialPara->size.width, dialPara->size.height,handPara->drawPicType);
    for(i= 0; i<=capacity - 1; i++)
    {
        minuteRadians = i * 6 * PI / 180;
//
        minuteHandRadius = handPara->size.width; // .MinuteHandRadius;
        minuteHandColor = LBM_KGColorToSdlColor(handPara->color);
        // .MinuteHandColor);
        if (minuteHandRadius == 0)
        {
            minuteHandRadius = 1;
            minuteHandColor = LBM_KGColorToSdlColor(KG_COLOR_BLACK);

        }
        _rectangularparadcb = KGW_GetPolygonRectangle(dialParaExt->_centerX,dialParaExt->_centerY,minuteHandRadius, _minuteLength,
                              minuteHandColor, minuteRadians);

        //LBM_ClearLedBitmap(backgroundBitmap);
//
        //KGW_DrawPolygon(handPara->handType, dialParaExt->_centerX,dialParaExt->_centerY,minuteHandRadius, _minuteLength, minuteHandColor,
        //                minuteRadians, backgroundBitmap,NULL);

//
        bitmap = LBM_CreateLedBitmap(_rectangularparadcb.Width,
                                     _rectangularparadcb.Height,handPara->drawPicType);
//
        rcbitmap.X = 0;
        rcbitmap.Y = 0;
        rcbitmap.Width = bitmap->w;
        rcbitmap.Height = bitmap->h;
        //LBM_CopyLebBitmap(backgroundBitmap,_rectangularparadcb,bitmap,rcbitmap);
        pLeft = LBM_MakePoint(_rectangularparadcb.X,_rectangularparadcb.Y);
        KGW_DrawPolygon(handPara->handType, dialParaExt->_centerX,dialParaExt->_centerY,minuteHandRadius, _minuteLength, minuteHandColor,
                        minuteRadians, bitmap,&pLeft);


        getFileName(fileNamePrefix,index,filename);
        if(i==0)
        {
            bmp0 = LBM_Clone(bitmap);
        }
        bw = bitmap->w;
        bh = bitmap->h;
        LBM_FreeLedBitmap(bitmap);
        bmp = rotozoomSurface(bmp0,-i*6,1,0);
        bitmap = LBM_CreateLedBitmap(bw,bh,KG_BMP_32BIT);
        LBM_Draw(bitmap,(bitmap->w-bmp->w)/2,(bitmap->h-bmp->h)/2,bmp);
        LBM_FreeLedBitmap(bmp);

        result = LBM_SaveLedBitmapToFile(bitmap,filename,&(watchHands[index].bitmap),createMode);

        watchHands[index].coord.x = _rectangularparadcb.X;
        watchHands[index].coord.y = _rectangularparadcb.Y;
        watchHands[index].coord.width = _rectangularparadcb.Width;
        watchHands[index].coord.height = _rectangularparadcb.Height;
        strcpy(watchHands[index].handContent,filename);

        index++;

        LBM_FreeLedBitmap(bitmap);
        if(result!=0)
        {
            break;
        }
    }
    LBM_FreeLedBitmap(backgroundBitmap);
    LBM_FreeLedBitmap(bmp0);
    return result;
}

int KGW_MakeSeconds(const DrawHandPara *handPara, const DrawDialPara *dialPara, const DrawDialParaExt *dialParaExt,
                    const char *fileNamePrefix, WatchHand watchHands[],KGCreateMode createMode)
{
    int result = -1;
    char filename[1024];
    Sint32 capacity;
    LedBitmap *bitmap,*backgroundBitmap,*bmp0,*bmp;
    int bw,bh;
    int index = 0;


    Sint32 i;
    float secondRadians;
    Sint32 secondHandRadius;
    Uint32 secondHandColor;
    RECTANGULARPARADCB _rectangularparadcb,rcbitmap;
    Uint32 _secondLength = handPara->size.height;
    LBM_Point pLeft;
    capacity = 60;
//
    backgroundBitmap = LBM_CreateLedBitmap(dialPara->size.width, dialPara->size.height,handPara->drawPicType);
    for(i= 0; i<=capacity - 1; i++)
    {
        secondRadians = i * 6 * PI / 180;
//    secondHandRadius := 1;
        secondHandRadius = handPara->size.width ;
        secondHandColor = LBM_KGColorToSdlColor(handPara->color);
        // .SecondHandColor);
        _rectangularparadcb = KGW_GetPolygonRectangle(dialParaExt->_centerX,dialParaExt->_centerY,secondHandRadius, _secondLength,
                              secondHandColor, secondRadians);

        //LBM_ClearLedBitmap(backgroundBitmap);
//
        //KGW_DrawPolygon(handPara->handType, dialParaExt->_centerX,dialParaExt->_centerY,secondHandRadius, _secondLength, secondHandColor,
        //                secondRadians, backgroundBitmap,NULL);

//
        bitmap = LBM_CreateLedBitmap(_rectangularparadcb.Width,
                                     _rectangularparadcb.Height,handPara->drawPicType);

        rcbitmap.X = 0;
        rcbitmap.Y = 0;
        rcbitmap.Width = bitmap->w;
        rcbitmap.Height = bitmap->h;
        //LBM_CopyLebBitmap(backgroundBitmap,_rectangularparadcb,bitmap,rcbitmap);
        pLeft = LBM_MakePoint(_rectangularparadcb.X,_rectangularparadcb.Y);
        KGW_DrawPolygon(handPara->handType, dialParaExt->_centerX,dialParaExt->_centerY,secondHandRadius, _secondLength, secondHandColor,
                        secondRadians, bitmap,&pLeft);


        getFileName(fileNamePrefix,index,filename);
        if(i==0)
        {
            bmp0 = LBM_Clone(bitmap);
        }
        bw = bitmap->w;
        bh = bitmap->h;
        LBM_FreeLedBitmap(bitmap);
        bmp = rotozoomSurface(bmp0,-i*6,1,0);
        bitmap = LBM_CreateLedBitmap(bw,bh,KG_BMP_32BIT);
        LBM_Draw(bitmap,(bitmap->w-bmp->w)/2,(bitmap->h-bmp->h)/2,bmp);
        LBM_FreeLedBitmap(bmp);

        result = LBM_SaveLedBitmapToFile(bitmap,filename,&(watchHands[index].bitmap),createMode);

        watchHands[index].coord.x = _rectangularparadcb.X;
        watchHands[index].coord.y = _rectangularparadcb.Y;
        watchHands[index].coord.width = _rectangularparadcb.Width;
        watchHands[index].coord.height = _rectangularparadcb.Height;
        strcpy(watchHands[index].handContent,filename);

        index++;

        LBM_FreeLedBitmap(bitmap);
        if(result!=0)
        {
            break;
        }
    }
    LBM_FreeLedBitmap(backgroundBitmap);
    LBM_FreeLedBitmap(bmp0);
    return result;
}


void KGW_DrawThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{

//
    if( drawPara->dialType == DIAL_CIRCLE)
    {
        KGW_MakeThicks(Graphics,drawPara,dialParaExt);
        //if( drawPara->IsShowDigital)
        //{
        //    KGW_MakeDigitals(Graphics,drawPara,dialParaExt);
        //}
    }
//
    if( drawPara->dialType == DIAL_SQUARE)
    {
        KGW_MakeRoundRectThicks(Graphics,drawPara,dialParaExt);
        //if( drawPara->IsShowDigital)
        //{
//
        //    KGW_MakeRoundRectDigitals(Graphics,drawPara,dialParaExt);
        //}
    }
//
    if( drawPara->dialType == DIAL_OVAL)
    {
        KGW_MakeEllipseThicks(Graphics,drawPara,dialParaExt);
        //if( drawPara->IsShowDigital)
        //{
//
        //   KGW_MakeEllipseDigitals(Graphics,drawPara,dialParaExt);
        //}

    }
//
    if( drawPara->dialType == DIAL_DIAMOND)
    {
        KGW_MakeRhombusThicks(Graphics,drawPara,dialParaExt);
        //if( drawPara->IsShowDigital)
        //{
//
        //    KGW_MakeRhombusDigitals(Graphics,drawPara,dialParaExt);
        //}
    }
//
    /*
    if( drawPara->dialType == TWatchStyle.WS_WINDMILL)
    {
      MakePolygon12Thicks(Graphics,drawPara,dialParaExt);
      if( drawPara->IsShowDigital)
      {
    //
        MakePolygon12Digitals(Graphics,drawPara,dialParaExt);
      }
    }
    */

    return;
}

/// <summary>
//
/// </summary>
/// <param name="graphics"></param>
//
//
//
void KGW_DrawTick(LedBitmap *Graphics, Uint32 color32, float centerX, float centerY,float _radius, float tickRadius, float offsetRaduis, float angle)
{
    Uint32 color;
    float ang = angle*PI/180;
    float x = centerX+(_radius - offsetRaduis)*cosf(ang);
    float y = centerY+(_radius - offsetRaduis)*sinf(ang);
    Sint32 sx = (Sint32)(x+0.5);
    Sint32 sy = (Sint32)(y+0.5);
    Sint32 r;
    if (tickRadius == 1)
    {
        KGW_DrawDot(Graphics, color32, centerX, centerY,_radius,offsetRaduis,angle);
        return;
    }

//
    color = color32;
    //graphicsPath = TGPGraphicsPath.Create();
    //graphicsPath.AddEllipse(-tickRadius / 2, -(_radius - offsetRaduis),
    //                        tickRadius, tickRadius);
    //filledEllipseColor(Graphics, sx, sy, tickRadius/2, tickRadius/2, color);
    r = (Sint32)(tickRadius/2);
    if(sx+r>=Graphics->w)
    {
        sx = Graphics->w-1-r;
    }
    if(sy+r>=Graphics->h)
    {
        sy = Graphics->h-1-r;
    }
    filledCircleColor(Graphics, sx, sy,r, color);
    circleColor(Graphics, sx, sy, r, color);
    //LBM_DrawCircle(Graphics, sx, sy, tickRadius/2,  color);
}

/// <summary>
//
/// </summary>
/// <param name="graphics"></param>
/// <param name="color32"></param>
/// <param name="radius"></param>
void KGW_DrawDot(LedBitmap *Graphics, Sint32 color32, float centerX, float centerY, float _radius, float radius,float angle)
{
    Uint32 color;
    LBM_PointF point;
    LBM_PointF point2;
    float ang = angle*PI/180;
    Sint32 sx;
    Sint32 sy;
//
    color = color32;
    //point := LBM_MakePointF(-1 / 2.0, -(_radius - radius));
    //point = LBM_MakePointF(0, Integer(-(_radius - radius)));
    //point2 = LBM_MakePointF(point.X + 1, point.Y);
    point.X = centerX+_radius*cosf(ang);
    point.Y = centerY+_radius*sinf(ang);
    point2.X = centerX+(_radius-radius)*cosf(ang);
    point2.Y = centerY+(_radius-radius)*sinf(ang);
    sx = (Sint32)(point2.X+0.5);
    sy = (Sint32)(point2.Y+0.5);
    //lineColor(Graphics,point.X,point.Y,point2.X,point2.Y,color);
    if(sx>=Graphics->w)
    {
        sx = Graphics->w-1;
    }
    if(sy>=Graphics->h)
    {
        sy = Graphics->h-1;
    }
    pixelColor(Graphics,sx,sy,color);

    //gpen = TGPPen.Create(MakeColor(color));
    //SetLength(points, 2);
    //points[0] = point2;
    //points[1] = point;
    //Graphics.DrawLines(gpen, PGPPointF(points), length(points));

    // graphics.DrawLine(gpen, 0, -_radius, 0, -(_radius - radius));


}

/// <summary>
//
/// </summary>
/// <returns></returns>
void KGW_MakeThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 offsetRadius;
    Sint32 i;
    float angle = 0;
    //TLedBitmap bitmap;
    //TRect rcText;
    //Sint32 xText;
    //Sint32 yText;
    //TGPBitmap gpbitmap;
    /*
    if (drawPara->EnabledText)
    {
        {
            if(drawPara->Text!='')
            {
                bitmap = TBitmap.Create;
                bitmap.Width = 10;
                bitmap.Height = 10;
                rcText = Rect(0,0,10,10);
                bitmap.Canvas.Font = drawPara->Font;
                DrawText(bitmap.Canvas.Handle, PChar(drawPara->Text), Length(drawPara->Text),
                         rcText, DT_CENTER or DT_VCENTER or DT_SINGLELINE or DT_CALCRECT);
                bitmap.Width = rcText.Right-rcText.Left+1;
                bitmap.Height = rcText.Bottom-rcText.Top+1;
                bitmap.Canvas.Brush.Color = TSystemConfigHandler.DefaultBackgroundColor;
                bitmap.Canvas.Brush.Style = bsSolid;
                bitmap.Canvas.FillRect(Rect(0,0,bitmap.Width,bitmap.Height));
                rcText = Rect(0,0,bitmap.Width,bitmap.Height);

                DrawText(bitmap.Canvas.Handle, PChar(drawPara->Text), Length(drawPara->Text),
                         rcText, DT_CENTER or DT_VCENTER or DT_SINGLELINE);


                xText = _centerX - bitmap.Width / 2;
                xText = xText-drawPara->DeltText.Left;
                xText = xText+drawPara->DeltText.Right;
                yText = _centerY + (_radius - bitmap.Height) / 2;
                yText = yText-drawPara->DeltText.Top;
                yText = yText+drawPara->DeltText.Bottom;
                gpbitmap = TGPBitmap.Create(bitmap.Handle,bitmap.Palette);

                graphics.DrawImage(gpbitmap,xText,yText);



            }
        }
        if (drawPara->Text != '')
        {
            bitmap = ClockMaker.DrawText(drawPara->Font, drawPara->Font.color,
                                         drawPara->Text, nil);
            if (bitmap != nil)
            {
                // if(!fixedTextInfo.ChangeLocation) then
                // begin
    //
                // fixedTextInfo.X := _centerX - bitmap.Width / 2;
                // fixedTextInfo.Y := _centerY + (_radius - bitmap.Height) / 2;
                // end;
                rcText = Rect(0, 0, bitmap.width, bitmap.height);
                xText = _centerX - bitmap.width / 2;
                xText = xText - drawPara->DeltText.left;
                xText = xText + drawPara->DeltText.right;
                yText = _centerY + (_radius - bitmap.height) / 2;
                yText = yText - drawPara->DeltText.top;
                yText = yText + drawPara->DeltText.bottom;
                Graphics.DrawImage(bitmap, xText, yText);
                FreeAndNil(bitmap);
            }
        }
    }
    */
//
    //Graphics.TranslateTransform(_centerX, _centerY);

    // if ((dialDrawMode and DialDrawMode.Ticks)=TDialDrawMode.Ticks) then
    {

        for(i= 0; i<=59; i++)
        {

            // offsetRadius := max(drawPara->Tick369Radius, drawPara->HourTickRadius) / 2;
            offsetRadius = max(drawPara->crossSize,
                               drawPara->hourSize) / 2;

            if (i % 5 == 0)
            {

                if( (i == 0) || (i == 15) || (i == 30) || (i == 45))
                {
//
                    int _radius = dialParaExt->_radius;
                    if(i==0 || i==15)
                    {
                        //_radius = dialParaExt->_radiusA;
                    }
                    if (drawPara->crossSize > 0)
                    {
                        if (drawPara->hourSize <
                                drawPara->crossSize)
                        {
                            //offsetRadius = 0;
                        }

                        KGW_DrawTick(Graphics, LBM_KGColorToSdlColor(drawPara->crossColor),
                                     dialParaExt->_centerX,dialParaExt->_centerY,_radius,
                                     drawPara->crossSize, offsetRadius,angle);
                    }
                }
                else
                {
//
                    if (drawPara->hourSize > 0)
                    {

                        if (drawPara->hourSize >
                                drawPara->crossSize)
                        {
                            //offsetRadius = 0;
                        }

                        KGW_DrawTick(Graphics, LBM_KGColorToSdlColor(drawPara->hourColor),
                                     dialParaExt->_centerX,dialParaExt->_centerY,dialParaExt->_radius,
                                     drawPara->hourSize, offsetRadius,angle);
                    }
                }
            }
            else
            {
                if (drawPara->quarterSize > 0)
                {
                    KGW_DrawDot(Graphics, LBM_KGColorToSdlColor(drawPara->quarterColor),
                                dialParaExt->_centerX,dialParaExt->_centerY,dialParaExt->_radius,
                                offsetRadius,angle);
                }
            }
            angle += 6.0;
            //Graphics.RotateTransform(6.0);
        }
    }
//
    //Graphics.ResetTransform();

}
/*
void KGW_MakeDigitals(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 maxInnerRadius;
    Sint32 maxTickDotRadius;
    Sint32 realInnerRadius;
    Sint32 i;
    Sint32 digitalArabic;
    string digitalRoamer;
    string digitalString;
    Uint32 color;
    TGPSolidBrush gpbrush;
    TLogFont lf;
    TGPFont Font;
    Sint32 digitalStringlength;
    TGPStringFormat stringFormat;
    LBM_PointF origin;
    TGPSizeF size;
    TGPRectF boundingRect;
    string digitalfontname;
    Sint32 digitalfontsize;
    float maxOfferset;
    HFONT fontHandle;
    LBM_PointF pointXY;
//
    maxTickDotRadius = max(drawPara->crossSize,
                           drawPara->hourSize) / 2;
//
    maxInnerRadius = min(drawPara->width, drawPara->height) / 2;
//
    realInnerRadius = maxInnerRadius - maxTickDotRadius + 2;
//
    Graphics.TranslateTransform(_centerX, _centerY);
//
    for(i= 1; i<=12; i++)
    {
        if (i % 3) == 0
        {
            color = LBM_KGColorToSdlColor(drawPara->crossColor);
        }
        else
        {
            color = LBM_KGColorToSdlColor(drawPara->hourColor);
        }
        //digitalString := Format('%d', [i]);
        if( drawPara->DigitalStyle==TInnerDigitalStyle.DS_ARABIC)
        {
            digitalString = Format('%d', [i]);
        }
        if(  drawPara->DigitalStyle==TInnerDigitalStyle.DS_ROAMER)
        {
            digitalString=RomanNum[i];
        }
        digitalStringlength = length(digitalString);
        digitalfontname = drawPara->DigitalFont.Name;
        digitalfontsize = drawPara->DigitalFont.size;
        Font = TGPFont.Create(digitalfontname, digitalfontsize);
        digitalStringlength = length(digitalString);
        stringFormat = TGPStringFormat.Create;
        stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
        stringFormat.SetLineAlignment(StringAlignmentNear);
        origin.X = 0;
        origin.Y = 0;
        Graphics.MeasureString(digitalString, digitalStringlength, Font, origin,
                               stringFormat, boundingRect);
        maxOfferset = max(boundingRect.height, boundingRect.width);
        pointXY.X = Sin((PI / 6) * i) * (realInnerRadius - maxOfferset);
        pointXY.Y = Cos((PI / 6) * i) * (realInnerRadius - maxOfferset);

        boundingRect.X = pointXY.X - boundingRect.width / 2;
        boundingRect.Y = -pointXY.Y - boundingRect.height / 2;

        gpbrush = TGPSolidBrush.Create(MakeColor(color));
        Graphics.DrawString(digitalString, length(digitalString), Font,
                            boundingRect, stringFormat, gpbrush);

        FreeAndNil(Font);
        FreeAndNil(stringFormat);
        FreeAndNil(gpbrush);
    }
//
    Graphics.ResetTransform();
}
}
*/
//
void KGW_MakeRoundRectThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 offsetRadius;
    Sint32 i;
    Sint32 quadrant;

    //TRect rcText;
    //Sint32 xText;
    //Sint32 yText;
    float radiusArray[15];//array [0 .. 14] of Single radiusArray;
    float angle;
    Sint32 tickRadius;

    Uint32 color;
    //TGPGraphicsPath graphicsPath;
    //TGPSolidBrush sb;
    //TGPPen gpen;
    LBM_PointF point;
    LBM_PointF point2;
    LBM_PointF points[2];
    /*
    if (drawPara->EnabledText)
    {
        if (drawPara->Text != '')
        {
            bitmap = ClockMaker.DrawText(drawPara->Font, drawPara->Font.color,
                                         drawPara->Text, nil);
            if (bitmap != nil)
            {
                // if(!fixedTextInfo.ChangeLocation) then
                // begin
    //
                // fixedTextInfo.X := _centerX - bitmap.Width / 2;
                // fixedTextInfo.Y := _centerY + (_radius - bitmap.Height) / 2;
                // end;
                rcText = Rect(0, 0, bitmap.width, bitmap.height);
                xText = _centerX - bitmap.width / 2;
                xText = xText - drawPara->DeltText.left;
                xText = xText + drawPara->DeltText.right;
                yText = _centerY + (_radius - bitmap.height) / 2;
                yText = yText - drawPara->DeltText.top;
                yText = yText + drawPara->DeltText.bottom;
                Graphics.DrawImage(bitmap, xText, yText);
                FreeAndNil(bitmap);
            }
        }
    }
    */
//
    //Graphics.TranslateTransform(_centerX, _centerY);
//
//
//
//
    offsetRadius = max(drawPara->crossSize,
                       drawPara->hourSize) / 2;

    for(i= 0; i<=14; i++)
    {
        if( i <= 5)
        {
//
            angle = (i * PI) / 30;
            radiusArray[i] = dialParaExt->_radius / cos(angle);
        }
        if( (i > 5) && (i <= 10))
        {
            radiusArray[i] = radiusArray[5];
        }
        if( i > 10)
        {
            radiusArray[14] = radiusArray[1];
            radiusArray[13] = radiusArray[2];
            radiusArray[12] = radiusArray[3];
            radiusArray[11] = radiusArray[4];
        }
    }
    angle = 0;
    for(quadrant= 1; quadrant<=4; quadrant++)
    {
        for(i= 0; i<=14; i++)
        {
            if( i % 15 == 0)
            {
//
                if (drawPara->crossSize > 0)
                {
                    if (drawPara->hourSize < drawPara->crossSize)
                    {
                        //offsetRadius = 0;
                    }
                    color = LBM_KGColorToSdlColor(drawPara->crossColor);
                    tickRadius = drawPara->crossSize;
                    //graphicsPath = TGPGraphicsPath.Create();
                    //graphicsPath.AddEllipse(-tickRadius / 2,
                    //                        -(radiusArray[i] - offsetRaduis), tickRadius, tickRadius);
                    //sb = TGPSolidBrush.Create(MakeColor(color));
                    //Graphics.FillPath(sb, graphicsPath);
                    //gpen = TGPPen.Create(MakeColor(color));
                    //Graphics.DrawPath(gpen, graphicsPath);
                    KGW_DrawTick(Graphics, color,
                                 dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                 drawPara->crossSize, offsetRadius,angle);
                }
            }
            else
            {
//
                if( i % 5 == 0)
                {
//
                    if (drawPara->hourSize > 0)
                    {
                        if (drawPara->hourSize >
                                drawPara->crossSize)
                        {
                            //offsetRadius = 0;
                        }
                        color = LBM_KGColorToSdlColor(drawPara->hourColor);
                        tickRadius = drawPara->hourSize;
                        //graphicsPath = TGPGraphicsPath.Create();
                        //graphicsPath.AddEllipse(-tickRadius / 2,
                        //                        -(radiusArray[i] - offsetRaduis), tickRadius, tickRadius);

                        //sb = TGPSolidBrush.Create(MakeColor(color));
                        //Graphics.FillPath(sb, graphicsPath);
                        //gpen = TGPPen.Create(MakeColor(color));
                        //Graphics.DrawPath(gpen, graphicsPath);
                        KGW_DrawTick(Graphics, color,
                                     dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                     drawPara->hourSize, offsetRadius,angle);
                    }
                }
                else
                {
//
                    if (drawPara->quarterSize > 0)
                    {
                        color = LBM_KGColorToSdlColor(drawPara->quarterColor);
                        //point = LBM_MakePointF(-1 / 2.0, -(radiusArray[i] - offsetRaduis));

                        //point2 = LBM_MakePointF(point.X + 1, point.Y);
                        //gpen = TGPPen.Create(MakeColor(color));
                        //SetLength(points, 2);
                        //points[0] = point2;
                        //points[1] = point;
                        //Graphics.DrawLines(gpen, PGPPointF(points), length(points));
                        // graphics.DrawLine(gpen, 0, -radiusArray[i], 0, -(radiusArray[i] - offsetRaduis));
                        // graphics.DrawLine(gpen, 0, -radiusArray[i], 0, -(radiusArray[i]-1));
                        KGW_DrawDot(Graphics, color,
                                    dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                    offsetRadius,angle);
                    }
                }
            }
            //Graphics.RotateTransform(6.0);
            angle += 6.0;
        }
        //Graphics.ResetTransform();
        //Graphics.TranslateTransform(_centerX, _centerY);
        //Graphics.RotateTransform(90.0 * quadrant);
    }
    //Graphics.ResetTransform();
}
/*
void KGW_MakeRoundRectDigitals(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 maxInnerRadius;
    Sint32 maxTickDotRadius;
    Sint32 realInnerRadius;
    Sint32 i;
    Sint32 digitalArabic;
    string digitalRoamer;
    string digitalString;
    Uint32 color;
    TGPSolidBrush gpbrush;
    TLogFont lf;
    TGPFont Font;
    Sint32 digitalStringlength;
    TGPStringFormat stringFormat;
    LBM_PointF origin;
    TGPSizeF size;
    TGPRectF boundingRect;
    string digitalfontname;
    Sint32 digitalfontsize;
    float maxOfferset;
    HFONT fontHandle;
    LBM_PointF pointXY;
//
    maxTickDotRadius = max(drawPara->crossSize,
                           drawPara->hourSize) / 2;
//
    maxInnerRadius = min(drawPara->width, drawPara->height) / 2;
//
    realInnerRadius = maxInnerRadius - maxTickDotRadius + 3;
//
    Graphics.TranslateTransform(_centerX, _centerY);
//
    for(i= 1; i<=12; i++)
    {
        if (i % 3) == 0
        {
            color = LBM_KGColorToSdlColor(drawPara->crossColor);
        }
        else
        {
            color = LBM_KGColorToSdlColor(drawPara->hourColor);
        }
        //digitalString := Format('%d', [i]);
        if( drawPara->DigitalStyle==TInnerDigitalStyle.DS_ARABIC)
        {
            digitalString = Format('%d', [i]);
        }
        if(  drawPara->DigitalStyle==TInnerDigitalStyle.DS_ROAMER)
        {
            digitalString=RomanNum[i];
        }
        digitalStringlength = length(digitalString);
        digitalfontname = drawPara->DigitalFont.Name;
        digitalfontsize = drawPara->DigitalFont.size;
        Font = TGPFont.Create(digitalfontname, digitalfontsize);
        digitalStringlength = length(digitalString);
        stringFormat = TGPStringFormat.Create;
        stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
        stringFormat.SetLineAlignment(StringAlignmentNear);

        origin.X = 0;
        origin.Y = 0;
        Graphics.MeasureString(digitalString, digitalStringlength, Font, origin,
                               stringFormat, boundingRect);
        maxOfferset = max(boundingRect.height, boundingRect.width);
        if (i % 3) == 0
        {
            pointXY.X = Sin((PI / 6) * i) * (realInnerRadius - maxOfferset);
            pointXY.Y = Cos((PI / 6) * i) * (realInnerRadius - maxOfferset);
        }
        else
        {
            pointXY.X = Sin((PI / 6) * i) *
                        (realInnerRadius * (1.732051 * 2 / 3) - maxOfferset);
            pointXY.Y = Cos((PI / 6) * i) *
                        (realInnerRadius * (1.732051 * 2 / 3) - maxOfferset);
        }
        boundingRect.X = pointXY.X - boundingRect.width / 2;
        boundingRect.Y = -pointXY.Y - boundingRect.height / 2;
        gpbrush = TGPSolidBrush.Create(MakeColor(color));
        Graphics.DrawString(digitalString, length(digitalString), Font,
                            boundingRect, stringFormat, gpbrush);
        FreeAndNil(Font);
        FreeAndNil(stringFormat);
        FreeAndNil(gpbrush);
    }
//
    Graphics.ResetTransform();
}
}
*/


/// ///////////////////////////////////////////////////
void KGW_MakeEllipseThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 offsetRadius;
    Sint32 i;
    Sint32 quadrant;
    //TLedBitmap bitmap;
    //TRect rcText;
    //Sint32 xText;
    //Sint32 yText;
    float radiusArray[30];
    float angle;
    Sint32 tickRadius;
    Uint32 color;

    LBM_PointF point;
    LBM_PointF point2;
    LBM_PointF points[2];
    Sint32 a;
    Sint32 b;
    /*
    if (drawPara->EnabledText)
    {
        if (drawPara->Text != '')
        {
            bitmap = ClockMaker.DrawText(drawPara->Font, drawPara->Font.color,
                                         drawPara->Text, nil);
            if (bitmap != nil)
            {
                // if(!fixedTextInfo.ChangeLocation) then
                // begin
    //
                // fixedTextInfo.X := _centerX - bitmap.Width / 2;
                // fixedTextInfo.Y := _centerY + (_radius - bitmap.Height) / 2;
                // end;
                rcText = Rect(0, 0, bitmap.width, bitmap.height);
                xText = _centerX - bitmap.width / 2;
                xText = xText - drawPara->DeltText.left;
                xText = xText + drawPara->DeltText.right;
                yText = _centerY + (_radius - bitmap.height) / 2;
                yText = yText - drawPara->DeltText.top;
                yText = yText + drawPara->DeltText.bottom;
                Graphics.DrawImage(bitmap, xText, yText);
                FreeAndNil(bitmap);
            }
        }
    }
    */
//
    //Graphics.TranslateTransform(_centerX, _centerY);

    offsetRadius = max(drawPara->crossSize,
                       drawPara->hourSize) / 2;


    a = drawPara->size.width / 2;
    b = drawPara->size.height / 2;
    if( (a <= 1) || (b <= 1))
    {
        return;
    }
    for(i= 0; i<=29; i++)
    {
        angle = (i * PI) / 30;
        radiusArray[i] = a * b / sqrt(a * a * sin(angle) * sin(angle) + b * b *
                                      cos(angle) * cos(angle));
    }
    angle = 0;
    for(quadrant= 1; quadrant<=2; quadrant++)
    {
        for(i= 0; i<=29; i++)
        {
            if ((i % 15) == 0)
            {
//
                if (drawPara->crossSize > 0)
                {
                    if (drawPara->hourSize < drawPara->crossSize)
                    {
                        //offsetRadius = 0;
                    }
                    color = LBM_KGColorToSdlColor(drawPara->crossColor);
                    tickRadius = drawPara->crossSize;
                    //graphicsPath = TGPGraphicsPath.Create();
                    //graphicsPath.AddEllipse(-(radiusArray[i] - offsetRadius),
                    //-tickRadius / 2, tickRadius, tickRadius);
                    //sb = TGPSolidBrush.Create(MakeColor(color));
                    //Graphics.FillPath(sb, graphicsPath);
                    //gpen = TGPPen.Create(MakeColor(color));
                    //Graphics.DrawPath(gpen, graphicsPath);
                    KGW_DrawTick(Graphics, color,
                                 dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                 drawPara->crossSize, offsetRadius,angle);
                }
            }
            else
            {
                if ((i % 5) == 0)
                {
//
                    if (drawPara->hourSize > 0)
                    {
                        if (drawPara->hourSize > drawPara->crossSize)
                        {
                            //offsetRadius = 0;
                        }
                        color = LBM_KGColorToSdlColor(drawPara->hourColor);
                        tickRadius = drawPara->hourSize;
                        //graphicsPath = TGPGraphicsPath.Create();
                        //graphicsPath.AddEllipse(-(radiusArray[i] - offsetRadius),
                        //-tickRadius / 2, tickRadius, tickRadius);
                        //sb = TGPSolidBrush.Create(MakeColor(color));
                        //Graphics.FillPath(sb, graphicsPath);
                        //gpen = TGPPen.Create(MakeColor(color));
                        //Graphics.DrawPath(gpen, graphicsPath);
                        KGW_DrawTick(Graphics, color,
                                     dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                     drawPara->hourSize, offsetRadius,angle);
                    }
                }
                else
                {
//
                    if (drawPara->quarterSize > 0)
                    {
                        color = LBM_KGColorToSdlColor(drawPara->quarterColor);
                        point = LBM_MakePointF(-(radiusArray[i] - offsetRadius), -1 / 2.0);

                        point2 = LBM_MakePointF(point.X + 1, point.Y);
                        //gpen = TGPPen.Create(MakeColor(color));
                        //SetLength(points, 2);
                        //points[0] = point2;
                        //points[1] = point;
                        //Graphics.DrawLines(gpen, PGPPointF(points), length(points));
                        //graphics.DrawLine(gpen, 0, -radiusArray[i], 0, -(radiusArray[i]-1));
                        KGW_DrawDot(Graphics, color,
                                    dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                    offsetRadius,angle);
                    }
                }
            }
            //Graphics.RotateTransform(6.0);
            angle += 6.0;
        }
        //Graphics.ResetTransform();
        //Graphics.TranslateTransform(_centerX, _centerY);
        //Graphics.RotateTransform(180.0 * quadrant);
    }
    //Graphics.ResetTransform();
}
/*
void KGW_MakeEllipseDigitals(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 maxInnerRadius;
    Sint32 maxTickDotRadius;
    Sint32 realInnerRadius;
    Sint32 i;
    Sint32 digitalArabic;
    string digitalRoamer;
    string digitalString;
    Uint32 color;
    TGPSolidBrush gpbrush;
    TLogFont lf;
    TGPFont Font;
    Sint32 digitalStringlength;
    TGPStringFormat stringFormat;
    LBM_PointF origin;
    TGPSizeF size;
    TGPRectF boundingRect;
    string digitalfontname;
    Sint32 digitalfontsize;
    float maxOfferset;
    HFONT fontHandle;
    LBM_PointF pointXY;
    Sint32 a;
    Sint32 b;
    float angle;
    array [1 .. 12] of Single radiusArray;
//
    maxTickDotRadius = max(drawPara->crossSize,
                           drawPara->hourSize) / 2;
//
//
//
//
    a = drawPara->size.height / 2;
    b = drawPara->size.width / 2;
    if( (a <= 1) || (b <= 1))
    {
        return;
    }
//
    Graphics.TranslateTransform(_centerX, _centerY);
//
    for(i= 1; i<=12; i++)
    {
//
        angle = (i * PI) / 6;

        maxInnerRadius = round(a * b / Sqrt(a * a * Sin(angle) * Sin(angle) + b *
                                            b * Cos(angle) * Cos(angle)));
//
        realInnerRadius = maxInnerRadius - maxTickDotRadius + 3;
        if (i % 3) == 0
        {
            color = LBM_KGColorToSdlColor(drawPara->crossColor);
        }
        else
        {
            color = LBM_KGColorToSdlColor(drawPara->hourColor);
        }
        //digitalString := Format('%d', [i]);
        if( drawPara->DigitalStyle==TInnerDigitalStyle.DS_ARABIC)
        {
            digitalString = Format('%d', [i]);
        }
        if(  drawPara->DigitalStyle==TInnerDigitalStyle.DS_ROAMER)
        {
            digitalString=RomanNum[i];
        }
        digitalStringlength = length(digitalString);
        digitalfontname = drawPara->DigitalFont.Name;
        digitalfontsize = drawPara->DigitalFont.size;
        Font = TGPFont.Create(digitalfontname, digitalfontsize);
        digitalStringlength = length(digitalString);
        stringFormat = TGPStringFormat.Create;
        stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
        stringFormat.SetLineAlignment(StringAlignmentNear);
        origin.X = 0;
        origin.Y = 0;
        Graphics.MeasureString(digitalString, digitalStringlength, Font, origin,
                               stringFormat, boundingRect);
        maxOfferset = max(boundingRect.height, boundingRect.width);

        pointXY.X = Sin((PI / 6) * i) * (realInnerRadius - maxOfferset);
        pointXY.Y = Cos((PI / 6) * i) * (realInnerRadius - maxOfferset);

        boundingRect.X = pointXY.X - boundingRect.width / 2;
        boundingRect.Y = -pointXY.Y - boundingRect.height / 2;

        gpbrush = TGPSolidBrush.Create(MakeColor(color));
        Graphics.DrawString(digitalString, length(digitalString), Font,
                            boundingRect, stringFormat, gpbrush);
        FreeAndNil(Font);
        FreeAndNil(stringFormat);
        FreeAndNil(gpbrush);
    }
//
    Graphics.ResetTransform();
}
}
*/
//
void KGW_MakeRhombusThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 offsetRadius;
    Sint32 i;
    Sint32 quadrant;
    //TLedBitmap bitmap;
    //TRect rcText;
    //Sint32 xText;
    //Sint32 yText;
    float radiusArray[30];
    float angle;
    Sint32 tickRadius;
    Uint32 color;
    //TGPGraphicsPath graphicsPath;
    //TGPSolidBrush sb;
    //TGPPen gpen;
    LBM_PointF point;
    LBM_PointF point2;
    LBM_PointF points[2];
    Sint32 a;
    Sint32 b;
    /*
    if (drawPara->EnabledText)
    {
        if (drawPara->Text != '')
        {
            bitmap = ClockMaker.DrawText(drawPara->Font, drawPara->Font.color,
                                         drawPara->Text, nil);
            if (bitmap != nil)
            {
                // if(!fixedTextInfo.ChangeLocation) then
                // begin
    //
                // fixedTextInfo.X := _centerX - bitmap.Width / 2;
                // fixedTextInfo.Y := _centerY + (_radius - bitmap.Height) / 2;
                // end;
                rcText = Rect(0, 0, bitmap.width, bitmap.height);
                xText = _centerX - bitmap.width / 2;
                xText = xText - drawPara->DeltText.left;
                xText = xText + drawPara->DeltText.right;
                yText = _centerY + (_radius - bitmap.height) / 2;
                yText = yText - drawPara->DeltText.top;
                yText = yText + drawPara->DeltText.bottom;
                Graphics.DrawImage(bitmap, xText, yText);
                FreeAndNil(bitmap);
            }
        }
    }
    */
//
    //Graphics.TranslateTransform(_centerX, _centerY);

    offsetRadius = max(drawPara->crossSize,
                       drawPara->hourSize) / 2;

    a = drawPara->size.width / 2;
    b = drawPara->size.height / 2;
    if( (a <= 1) || (b <= 1))
    {
        return;
    }
    for(i= 0; i<=29; i++)
    {
        angle = (i * PI) / 30;
        if( i <= 14)
        {
            radiusArray[i] = a * b / (a * sin(angle) + b * cos(angle));
        }
        if( i > 14)
        {
            radiusArray[i] = a * b / (a * sin(angle) - b * cos(angle));
        }
    }
    angle = 0;
    for(quadrant= 1; quadrant<=2; quadrant++)
    {
        for(i= 0; i<=29; i++)
        {
            if ((i % 15) == 0)
            {
//
                if (drawPara->crossSize > 0)
                {
                    if (drawPara->hourSize < drawPara->crossSize)
                    {
                        //offsetRadius = 0;
                    }
                    color = LBM_KGColorToSdlColor(drawPara->crossColor);
                    tickRadius = drawPara->crossSize;
                    //graphicsPath = TGPGraphicsPath.Create();
                    //graphicsPath.AddEllipse(-(radiusArray[i] - offsetRadius),
                    //-tickRadius / 2, tickRadius, tickRadius);
                    //sb = TGPSolidBrush.Create(MakeColor(color));
                    //Graphics.FillPath(sb, graphicsPath);
                    //gpen = TGPPen.Create(MakeColor(color));
                    //Graphics.DrawPath(gpen, graphicsPath);
                    KGW_DrawTick(Graphics, color,
                                 dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                 drawPara->crossSize, offsetRadius,angle);
                }
            }
            else
            {
                if ((i % 5) == 0)
                {
//
                    if (drawPara->hourSize > 0)
                    {
                        if (drawPara->hourSize > drawPara->crossSize)
                        {
                            //offsetRadius = 0;
                        }
                        color = LBM_KGColorToSdlColor(drawPara->hourColor);
                        tickRadius = drawPara->hourSize;
                        //graphicsPath = TGPGraphicsPath.Create();
                        //graphicsPath.AddEllipse(-(radiusArray[i] - offsetRadius),
                        //-tickRadius / 2, tickRadius, tickRadius);
                        //sb = TGPSolidBrush.Create(MakeColor(color));
                        //Graphics.FillPath(sb, graphicsPath);
                        //gpen = TGPPen.Create(MakeColor(color));
                        //Graphics.DrawPath(gpen, graphicsPath);
                        KGW_DrawTick(Graphics, color,
                                     dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                     drawPara->hourSize, offsetRadius,angle);
                    }
                }
                else
                {
//
                    if (drawPara->quarterSize > 0)
                    {
                        color = LBM_KGColorToSdlColor(drawPara->quarterColor);
                        point = LBM_MakePointF(-(radiusArray[i] - offsetRadius), -1 / 2.0);

                        point2 = LBM_MakePointF(point.X + 1, point.Y);
                        //gpen = TGPPen.Create(MakeColor(color));
                        //SetLength(points, 2);
                        //points[0] = point2;
                        //points[1] = point;
                        //Graphics.DrawLines(gpen, PGPPointF(points), length(points));
                        //graphics.DrawLine(gpen, 0, -radiusArray[i], 0, -(radiusArray[i]-1));
                        KGW_DrawDot(Graphics, color,
                                    dialParaExt->_centerX,dialParaExt->_centerY,radiusArray[i],
                                    offsetRadius,angle);
                    }
                }
            }
            //Graphics.RotateTransform(6.0);
            angle += 6.0;
        }
        //Graphics.ResetTransform();
        //Graphics.TranslateTransform(_centerX, _centerY);
        //Graphics.RotateTransform(180.0 * quadrant);
    }
    //Graphics.ResetTransform();
}
/*
void KGW_MakeRhombusDigitals(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 maxInnerRadius;
    Sint32 maxTickDotRadius;
    Sint32 realInnerRadius;
    Sint32 i;
    Sint32 digitalArabic;
    string digitalRoamer;
    string digitalString;
    Uint32 color;
    TGPSolidBrush gpbrush;
    TLogFont lf;
    TGPFont Font;
    Sint32 digitalStringlength;
    TGPStringFormat stringFormat;
    LBM_PointF origin;
    TGPSizeF size;
    TGPRectF boundingRect;
    string digitalfontname;
    Sint32 digitalfontsize;
    float maxOfferset;
    HFONT fontHandle;
    LBM_PointF pointXY;
    Sint32 a;
    Sint32 b;
    float angle;
    array [1 .. 12] of Single radiusArray;
//
    maxTickDotRadius = max(drawPara->crossSize,
                           drawPara->hourSize) / 2;
//
//
//
//
a = drawPara->size.height / 2;
    b = drawPara->size.width / 2;
    if( (a <= 1) || (b <= 1))
    {
        return;
    }

//
    Graphics.TranslateTransform(_centerX, _centerY);
//
    for(i= 1; i<=12; i++)
    {
//
        angle = (i * PI) / 6;
        if( i <= 3)
        {
            maxInnerRadius = round(a * b / (a * Sin(angle) + b * Cos(angle)));
        }
        if( (i > 3) && (i <= 6))
        {
            maxInnerRadius = round(a * b / (a * Sin(angle) - b * Cos(angle)));
        }
        if( (i > 6) && (i <= 9))
        {
//
            maxInnerRadius =
                round(a * b / (a * Cos(1.5 * PI - angle) + b *
                               Sin(1.5 * PI - angle)));
        }
        if( i > 9)
        {
            maxInnerRadius = round(a * b / (b * Cos(angle) - a * Sin(angle)));
        }
//
        realInnerRadius = maxInnerRadius - maxTickDotRadius + 3;
        if (i % 3) == 0
        {
            color = LBM_KGColorToSdlColor(drawPara->crossColor);
        }
        else
        {
            color = LBM_KGColorToSdlColor(drawPara->hourColor);
        }

        //digitalString := Format('%d', [i]);
        if( drawPara->DigitalStyle==TInnerDigitalStyle.DS_ARABIC)
        {
            digitalString = Format('%d', [i]);
        }
        if(  drawPara->DigitalStyle==TInnerDigitalStyle.DS_ROAMER)
        {
            digitalString=RomanNum[i];
        }
        digitalStringlength = length(digitalString);
        digitalfontname = drawPara->DigitalFont.Name;
        digitalfontsize = drawPara->DigitalFont.size;
        Font = TGPFont.Create(digitalfontname, digitalfontsize);
        digitalStringlength = length(digitalString);
        stringFormat = TGPStringFormat.Create;
        stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
        stringFormat.SetLineAlignment(StringAlignmentNear);
        origin.X = 0;
        origin.Y = 0;
        Graphics.MeasureString(digitalString, digitalStringlength, Font, origin,
                               stringFormat, boundingRect);
        maxOfferset = max(boundingRect.height, boundingRect.width);
        pointXY.X = Sin((PI / 6) * i) * (realInnerRadius - maxOfferset);
        pointXY.Y = Cos((PI / 6) * i) * (realInnerRadius - maxOfferset);
        boundingRect.X = pointXY.X - boundingRect.width / 2;
        boundingRect.Y = -pointXY.Y - boundingRect.height / 2;
        gpbrush = TGPSolidBrush.Create(MakeColor(color));
        Graphics.DrawString(digitalString, length(digitalString), Font,
                            boundingRect, stringFormat, gpbrush);
        FreeAndNil(Font);
        FreeAndNil(stringFormat);
        FreeAndNil(gpbrush);
    }
//
    Graphics.ResetTransform();
}
}

void KGW_MakePolygon12Thicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 offsetRadius;
    Sint32 i;
    Sint32 quadrant;
    TLedBitmap bitmap;
    TRect rcText;
    Sint32 xText;
    Sint32 yText;
    array [0 .. 14] of Single radiusArray;
    float angle;
    Sint32 tickRadius;
    Sint32 offsetRaduis;
    Uint32 color;
    TGPGraphicsPath graphicsPath;
    TGPSolidBrush sb;
    TGPPen gpen;
    LBM_PointF point;
    LBM_PointF point2;
    LBM_PointFDynArray points;
    Sint32 a;
    Sint32 b;
    if (drawPara->EnabledText)
    {
        if (drawPara->Text != '')
        {
            bitmap = ClockMaker.DrawText(drawPara->Font, drawPara->Font.color,
                                         drawPara->Text, nil);
            if (bitmap != nil)
            {
                // if(!fixedTextInfo.ChangeLocation) then
                // begin
//
                // fixedTextInfo.X := _centerX - bitmap.Width / 2;
                // fixedTextInfo.Y := _centerY + (_radius - bitmap.Height) / 2;
                // end;
                rcText = Rect(0, 0, bitmap.width, bitmap.height);
                xText = _centerX - bitmap.width / 2;
                xText = xText - drawPara->DeltText.left;
                xText = xText + drawPara->DeltText.right;
                yText = _centerY + (_radius - bitmap.height) / 2;
                yText = yText - drawPara->DeltText.top;
                yText = yText + drawPara->DeltText.bottom;
                Graphics.DrawImage(bitmap, xText, yText);
                FreeAndNil(bitmap);
            }
        }
    }
//
    Graphics.TranslateTransform(_centerX, _centerY);
    offsetRaduis = max(drawPara->crossSize,
                       drawPara->hourSize) / 2;

    for(i= 0; i<=14; i++)
    {
        if( i < 5)
        {
//
            angle = (i * PI) / 30;
            radiusArray[i] = _radius * 1.732051 / (1.732051 * Cos(angle) + Sin(angle));
        }
        if( (i >= 5) && (i < 10))
        {
            radiusArray[i] = radiusArray[i - 5];
        }
        if( i >= 10)
        {
            radiusArray[i] = radiusArray[i - 10];
        }
    }

    for(quadrant= 1; quadrant<=4; quadrant++)
    {
        for(i= 0; i<=14; i++)
        {
            if( i % 15 == 0)
            {
//
                if (drawPara->crossSize > 0)
                {
                    if (drawPara->hourSize < drawPara->crossSize)
                    {
                        offsetRadius = 0;
                    }
                    color = LBM_KGColorToSdlColor(drawPara->crossColor);
                    tickRadius = drawPara->crossSize;
                    graphicsPath = TGPGraphicsPath.Create();
                    // graphicsPath.AddEllipse(-tickRadius / 2,
                    // -(radiusArray[i] - offsetRaduis), tickRadius, tickRadius);
                    graphicsPath.AddEllipse(-radiusArray[i] - offsetRaduis,
                                            -tickRadius / 2, tickRadius, tickRadius);
                    sb = TGPSolidBrush.Create(MakeColor(color));
                    Graphics.FillPath(sb, graphicsPath);
                    gpen = TGPPen.Create(MakeColor(color));
                    Graphics.DrawPath(gpen, graphicsPath);
                }
            }
            else
            {
//
                if( i % 5 == 0)
                {
//
                    if (drawPara->hourSize > 0)
                    {
                        if (drawPara->hourSize >
                                drawPara->crossSize)
                        {
                            offsetRadius = 0;
                        }
                        color = LBM_KGColorToSdlColor(drawPara->hourColor);
                        tickRadius = drawPara->hourSize;
                        graphicsPath = TGPGraphicsPath.Create();
                        // graphicsPath.AddEllipse(-tickRadius / 2,
                        // -(radiusArray[i] - offsetRaduis), tickRadius, tickRadius);
                        graphicsPath.AddEllipse(-radiusArray[i] - offsetRaduis,
                                                -tickRadius / 2, tickRadius, tickRadius);
                        sb = TGPSolidBrush.Create(MakeColor(color));
                        Graphics.FillPath(sb, graphicsPath);
                        gpen = TGPPen.Create(MakeColor(color));
                        Graphics.DrawPath(gpen, graphicsPath);
                    }
                }
                else
                {
//
                    if (drawPara->quarterSize > 0)
                    {
                        color = LBM_KGColorToSdlColor(drawPara->quarterColor);
                        // point := LBM_MakePointF(-1 / 2.0, -(radiusArray[i] - offsetRaduis));
                        point = LBM_MakePointF(-radiusArray[i] - offsetRaduis, -1 / 2.0);

                        point2 = LBM_MakePointF(point.X + 1, point.Y);
                        gpen = TGPPen.Create(MakeColor(color));
                        SetLength(points, 2);
                        points[0] = point2;
                        points[1] = point;
                        Graphics.DrawLines(gpen, PGPPointF(points), length(points));
                        //graphics.DrawLine(gpen, 0, -radiusArray[i], 0, -(radiusArray[i]-1));
                    }
                }
            }
            Graphics.RotateTransform(6.0);
        }
        Graphics.ResetTransform();
        Graphics.TranslateTransform(_centerX, _centerY);
        Graphics.RotateTransform(90.0 * quadrant);
    }
    Graphics.ResetTransform();
}

void KGW_MakePolygon12Digitals(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt)
{
    Sint32 maxInnerRadius;
    Sint32 maxTickDotRadius;
    Sint32 realInnerRadius;
    Sint32 i;
    Sint32 digitalArabic;
    string digitalRoamer;
    string digitalString;
    Uint32 color;
    TGPSolidBrush gpbrush;
    TLogFont lf;
    TGPFont Font;
    Sint32 digitalStringlength;
    TGPStringFormat stringFormat;
    LBM_PointF origin;
    TGPSizeF size;
    TGPRectF boundingRect;
    string digitalfontname;
    Sint32 digitalfontsize;
    float maxOfferset;
    HFONT fontHandle;
    LBM_PointF pointXY;
//
    maxTickDotRadius = max(drawPara->crossSize,
                           drawPara->hourSize) / 2;
//
    maxInnerRadius = min(drawPara->width, drawPara->height) / 2;
//
    realInnerRadius = maxInnerRadius - maxTickDotRadius + 2;
//
    Graphics.TranslateTransform(_centerX, _centerY);
//
    for(i= 1; i<=12; i++)
    {
        if (i % 3) == 0
        {
            color = LBM_KGColorToSdlColor(drawPara->crossColor);
        }
        else
        {
            color = LBM_KGColorToSdlColor(drawPara->hourColor);
        }
        //digitalString := Format('%d', [i]);
        if( drawPara->DigitalStyle==TInnerDigitalStyle.DS_ARABIC)
        {
            digitalString = Format('%d', [i]);
        }
        if(  drawPara->DigitalStyle==TInnerDigitalStyle.DS_ROAMER)
        {
            digitalString=RomanNum[i];
        }
        digitalStringlength = length(digitalString);
        digitalfontname = drawPara->DigitalFont.Name;
        digitalfontsize = drawPara->DigitalFont.size;
        Font = TGPFont.Create(digitalfontname, digitalfontsize);
        digitalStringlength = length(digitalString);
        stringFormat = TGPStringFormat.Create;
        stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);
        stringFormat.SetLineAlignment(StringAlignmentNear);
        origin.X = 0;
        origin.Y = 0;
        Graphics.MeasureString(digitalString, digitalStringlength, Font, origin,
                               stringFormat, boundingRect);
        maxOfferset = max(boundingRect.height, boundingRect.width);
        pointXY.X = Sin((PI / 6) * i) * (realInnerRadius - maxOfferset);
        pointXY.Y = Cos((PI / 6) * i) * (realInnerRadius - maxOfferset);
        boundingRect.X = pointXY.X - boundingRect.width / 2;
        boundingRect.Y = -pointXY.Y - boundingRect.height / 2;
        gpbrush = TGPSolidBrush.Create(MakeColor(color));
        Graphics.DrawString(digitalString, length(digitalString), Font,
                            boundingRect, stringFormat, gpbrush);
        FreeAndNil(Font);
        FreeAndNil(stringFormat);
        FreeAndNil(gpbrush);
    }
//
    Graphics.ResetTransform();
}
}
*/
