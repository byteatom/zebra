#include "DynamicBackgroundEffectBase.h"

/*
void DB_Action_GetGraphicsPath(LedBitmap *bitmap, int index, int MaxD, int elementIndex, LBM_RectF *prectangleF,KGColor color)
{


    switch(elementIndex)
    {


    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 13: // 非封闭正弦线
    {
        Result = DB_Action_DrawSinPath(bitmap,index, MaxD, false,prectangleF);
        break;
    }
    case 4: // 封闭正弦线
    {
        Result = DB_Action_DrawSinPath(bitmap,index, MaxD, true,prectangleF);
        break;
    }
    case 7:
    case 8: // 双菱形
    {
        Result = DB_Action_DrawDoubleDiamondPath(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 9:
    case 10://4叶三角形1
    {
        Result = DB_Action_DrawFourTrianglePath(bitmap,index, MaxD, 0,prectangleF);
        break;
    }
    case 11:
    case 12://4叶三角形2
    {
        Result = DB_Action_DrawFourTrianglePath(bitmap,index, MaxD, 1,prectangleF);
        break;
    }
    case 14:
    case 15://4交叉圆
    {
        Result = DB_Action_DrawFourCirclePath(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 16://音乐
    {
        Result = DB_Action_DrawMusicPath(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 17:
    case 18://三角形
    {
        Result = DB_Action_DrawTrianglePath(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 19:
    case 20://六角星
    {
        Result = DB_Action_DrawSixStarPath(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 21://立方体
    {
        Result = DB_Action_DrawCubePath(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 22://circle
    {
        Result = DB_Action_DrawCircle(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 23://rect
    {
        Result = DB_Action_DrawBlock(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 24://diamond
    {
        Result = DB_Action_DrawDiamond(bitmap,index, MaxD,prectangleF);
        break;
    }
    case 25://five star
    {
        Result = DB_Action_DrawFiveStar(bitmap,index, MaxD,prectangleF);
        break;
    }
    default:
    {
        graphicsPath = TGPGraphicsPath.Create();
        graphicsPath.AddEllipse((MaxD - index) / 2.0, (MaxD - index) / 2.0,
        index, index);
        Result = TListGPGraphicsPath.Create;
        Result.Add(graphicsPath);
        graphicsPath.GetBounds(rectangleF);
        break;
    }
    }
}

LedBitmap *DB_Action_GetGraphicsBitmap(const BackgroundInput *input, int index, int MaxD, LBM_RectF *prectangleF, int *pelementIndex)
{
    TListGPGraphicsPath graphicsPaths;
    TGPGraphicsPath graphicsPath;
    TGPGraphics Graphics;
    TGPPen gppen;
    TGPSolidBrush gbrush;
    TLedBitmap tmpbmp;
    TGpImageAttributes Attr;
    int I;
    TBitmap bmp;
    TEasyLedColor color;
    color.Bitmap = nil;
    color.CopyFrom(ledcolor);
    //产生随机色
    if( color.Random)
    {
        color = TLedColorPicker.MakeAutoColor(FProgramInfo.ColorMode,FProgramInfo.ColorGrayed);
    }
    if( color.Bitmap!=nil)
    {
        //花色
        //这里先置为白色，后面填花色用
        color.Color = clWhite;
    }
    Attr = TGpImageAttributes.Create;
    Attr.SetColorKey(TLedColor.TLedColor(TEasyLedColor.TransparentColor).AColor,
                     TLedColor.TLedColor(TEasyLedColor.TransparentColor).AColor); // 设置ColorKey
    gbrush = TGPSolidBrush.Create(TLedColor.TLedColor(color.color).AColor);

    if( elementIndex<0)
    {
        //RandSeed := GetTickCount;
        Randomize();
        elementIndex = elements[Random(elements.Count)];
    }

    Result = TLedBitmap.Create(MaxD, MaxD);
    Graphics = Graphics.FromImage(Result);
    Graphics.Clear(TColorFilter.Instance.RealTransparentColor.AColor);
    gppen = TGPPen.Create(TLedColor.TLedColor(color.color).AColor);

    graphicsPaths = GetGraphicsPath(bitmap,index, MaxD, elementIndex,prectangleF);


    for(I= 0; I<=graphicsPaths.Count - 1; I++)
    {



        Graphics.DrawPath(gppen, graphicsPaths[I]);

        if (elementIndex == 8)
            or (elementIndex == 10)
            or (elementIndex == 12)
            or (elementIndex == 18)
            or (elementIndex == 20)
        {
            //填充
            Graphics.FillPath(gbrush, graphicsPaths[I]);
        }
    }
    if (elementIndex == 15)
    {
        //4交叉圆
        graphicsPath = TGPGraphicsPath.Create();
        for(I= 0; I<=graphicsPaths.Count - 1; I++)
        {
            graphicsPath.AddPath(graphicsPaths[I],true);
        }
        //填充
        Graphics.FillPath(gbrush, graphicsPath);
        FreeAndNil(graphicsPath);
    }




    //
    // Graphics.FillPath(gbrush, graphicsPath);

    // BoxingBitmap(bitmap);
    // FreeAndNil(gbrush);

    FreeAndNil(Graphics);
    FreeAndNil(graphicsPaths);

    // 这里对做一些图片变化
    if( elementIndex == 1)
    {
        // 正弦旋转90度
        tmpbmp = DynamicImageMaker.Rotation(Result, 90);
        FreeAndNil(Result);
        Result = tmpbmp;
    }
    else if (elementIndex == 2) or (elementIndex == 3)
    {
        // 正弦与镜像正弦叠加
        tmpbmp = TLedBitmap(Result.Clone(MakeRect(Integer(0), Integer(0),
                                         Integer(Result.Width), Integer(Result.Height)),
                                         Result.GetPixelFormat));
        DynamicImageMaker.Flip(tmpbmp, 1);

        Graphics = TGPGraphics.Create(Result);

        Graphics.DrawImage(tmpbmp, MakeRect(0, 0, tmpbmp.Width, tmpbmp.Height), 0,
                           0, tmpbmp.Width, tmpbmp.Height, unitPixel, Attr);

        FreeAndNil(tmpbmp);
        FreeAndNil(Graphics);

        if (elementIndex == 3)
        {
            // 旋转90
            tmpbmp = DynamicImageMaker.Rotation(Result, 90);
            FreeAndNil(Result);
            Result = tmpbmp;
        }
    }
    else if (elementIndex == 4) or (elementIndex == 5)
    {
        // 花形正弦与之旋转90度，叠加

        // 旋转度
        tmpbmp = DynamicImageMaker.Rotation(Result, 90);

        // 叠加
        Graphics = TGPGraphics.Create(Result);

        Graphics.DrawImage(tmpbmp, MakeRect(0, 0, tmpbmp.Width, tmpbmp.Height), 0,
                           0, tmpbmp.Width, tmpbmp.Height, unitPixel, Attr);

        FreeAndNil(tmpbmp);
        FreeAndNil(Graphics);

    }
    else if (elementIndex == 6)
    {
        // 花形正弦与之旋转（45，90，135）度，叠加

        Graphics = TGPGraphics.Create(Result);
        for(I= 1; I<=3; I++)
        {
            // 旋转90度
            tmpbmp = DynamicImageMaker.Rotation(Result, 45 * I);

            // 叠加
            Graphics.DrawImage(tmpbmp, MakeRect(0, 0, tmpbmp.Width, tmpbmp.Height),
                               0, 0, tmpbmp.Width, tmpbmp.Height, unitPixel, Attr);
            FreeAndNil(tmpbmp);
        }

        FreeAndNil(Graphics);

    }
    else if (elementIndex == 13)
    {
        // 音乐，正弦加2圆
        Graphics = TGPGraphics.Create(Result);
        // 画圆

        // 左边
        graphicsPath = TGPGraphicsPath.Create();
        graphicsPath.AddEllipse((MaxD - index) / 2.0, (MaxD - index) / 2.0,
                                0.5 * index, 0.5 * index);
        Graphics.DrawPath(gppen, graphicsPath);
        Graphics.FillPath(gbrush, graphicsPath);
        FreeAndNil(graphicsPath);

        // 右边
        graphicsPath = TGPGraphicsPath.Create();
        graphicsPath.AddEllipse(MaxD - (MaxD - index) / 2.0 - 0.5 * index,
                                MaxD - (MaxD - index) / 2.0 - 0.5 * index, 0.5 * index, 0.5 * index);
        Graphics.DrawPath(gppen, graphicsPath);
        Graphics.FillPath(gbrush, graphicsPath);
        FreeAndNil(graphicsPath);

        FreeAndNil(Graphics);

    }

    FreeAndNil(gppen);
    FreeAndNil(gbrush);
    //如果是花色，则填充花色
    if( not color.IsBaseColor)
    {
        bmp = TBitmap.Create;
        bmp.Width = Result.Width;
        bmp.Height = Result.Height;
        bmp.Canvas.Brush.Color = clBlack;
        bmp.Canvas.FillRect(Rect(0, 0, bmp.Width, bmp.Height));
        Graphics = TGPGraphics.Create(bmp.Canvas.Handle);
        //Graphics.DrawImage(Result,0,0);
        Graphics.DrawImage(Result, MakeRect(0, 0, bmp.Width, bmp.Height),
                           0, 0, bmp.Width, bmp.Height, unitPixel, Attr);
        FreeAndNil(Graphics);


        TLedColorPicker.FillCustumColor(bmp,
                                        Rect(0,0,bmp.Width,bmp.Height),
                                        color,
                                        not ((FProgramInfo.ColorMode==TEasyColorMode.COLORM_SEVEN)
                                             and (FProgramInfo.ColorGrayed==TEasyColorGrayed.COLORG_256)));



        tmpbmp = TLedBitmap.Create(bmp.Handle,bmp.Palette);

        FreeAndNil(Attr);
        Attr = TGpImageAttributes.Create;
        Attr.SetColorKey(TLedColor.TLedColor(clBlack).AColor,
                         TLedColor.TLedColor(clBlack).AColor); // 设置ColorKey

        Graphics = TGPGraphics.Create(Result);
        Graphics.DrawImage(tmpbmp, MakeRect(0, 0, bmp.Width, bmp.Height),
                           0, 0, bmp.Width, bmp.Height, unitPixel, Attr);
        FreeAndNil(Graphics);
        FreeAndNil(tmpbmp);
        FreeAndNil(bmp);

    }
    FreeAndNil(Attr);
    color.Free;

}

// 正弦线
void DB_Action_DrawSinPath(LedBitmap *bitmap, int index, int MaxD, bool closed, LBM_RectF *prectangleF,KGColor color)
{

    int x;
    double y;
    double a;
    int I;
    double *vxs = (double *)malloc((index+2)*sizeof(double));
    double *vys = (double *)malloc((index+2)*sizeof(double));

    vxs[0] = (MaxD - index) / 2.0 + 0;
    vys[0] = (MaxD - index) / 2.0 + index / 2;
    if( index > 0)
    {

        for(x= 0; x<=index; x++)
        {
            a = (x / index) * 2 * PI;
            y = Sin(a);
            y = (1 - y) * index / 2;
            vxs[0] = (MaxD - index) / 2.0 + x;
            vys[0] = (MaxD - index) / 2.0 + y;

        }
    }

// graphicsPath.AddBeziers()
    bezierColor(bitmap,vxs,vys,index+2,1,LBM_KGColorToSdlColor(color));
    if( closed)
    {

        //graphicsPath.AddClosedCurve(PGPPointF(points), Length(points));
    }
    else
    {
        //graphicsPath.AddCurve(PGPPointF(points), Length(points));
    }
    free(vxs);
    free(vys);


}

// 双菱形
void DB_Action_DrawDoubleDiamondPath(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
{
    Double left;
    Double top;
    TGPPointFDynArray points;
    TGPGraphicsPath graphicsPath;
    TGPGraphicsPath tmpgraphicsPath;
    Result = TListGPGraphicsPath.Create;
    tmpgraphicsPath = TGPGraphicsPath.Create;
    left = (MaxD - index) / 2.0;
    top = left;
    // 菱形。
    SetLength(points, 4);
    graphicsPath = TGPGraphicsPath.Create();
    points[0] = MakePointF(left, top + index / 2.0);
    points[1] = MakePointF(left + 0.333 * index, top + 0.8333 * index);
    points[2] = MakePointF(left + 0.6667 * index, top + index / 2.0);
    points[3] = MakePointF(left + 0.333 * index, top + 0.1667 * index);
    graphicsPath.AddPolygon(PGPPointF(points), Length(points));
    tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
    Result.Add(graphicsPath);

    graphicsPath = TGPGraphicsPath.Create();
    points[0] = MakePointF(MaxD - left, top + index / 2.0);
    points[1] = MakePointF(MaxD - (left + 0.333 * index), top + 0.8333 * index);
    points[2] = MakePointF(MaxD - (left + 0.6667 * index), top + index / 2.0);
    points[3] = MakePointF(MaxD - (left + 0.333 * index), top + 0.1667 * index);
    graphicsPath.AddPolygon(PGPPointF(points), Length(points));
    tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
    Result.Add(graphicsPath);

    tmpgraphicsPath.GetBounds(rectangleF);
    FreeAndNil(tmpgraphicsPath);
}
//4叶三角形
DB_Action_DrawFourTrianglePath(LedBitmap *bitmap, int index, int MaxD, int mode, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    Double left;
    Double top;
    TGPPointFDynArray allpoints;
    TGPPointFDynArray points;
    TGPGraphicsPath graphicsPath;
    TGPGraphicsPath tmpgraphicsPath;
    Result = TListGPGraphicsPath.Create;
    tmpgraphicsPath = TGPGraphicsPath.Create;
    left = (MaxD - index) / 2.0;
    top = left;
    //选计算出所有有用的点
    SetLength(allpoints,12);
    allpoints[0] = MakePointF(left, top);
    allpoints[1] = MakePointF(left+0.2*index, top);
    allpoints[2] = MakePointF(left+0.8*index, top);
    allpoints[3] = MakePointF(left+index, top);

    allpoints[4] = MakePointF(left+index, top+0.2*index);
    allpoints[5] = MakePointF(left+index, top+0.8*index);
    allpoints[6] = MakePointF(left+index, top+index);

    allpoints[7] = MakePointF(left+0.8*index, top+index);
    allpoints[8] = MakePointF(left+0.2*index, top+index);
    allpoints[9] = MakePointF(left, top+index);

    allpoints[10] = MakePointF(left, top+0.8*index);
    allpoints[11] = MakePointF(left, top+0.2*index);

    if( mode==0)
    {
        SetLength(points, 4);
        //中间点
        points[0] = MakePointF(left+index /2.0, top + index / 2.0);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[11];
        points[2] = allpoints[0];
        points[3] = allpoints[1];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[2];
        points[2] = allpoints[3];
        points[3] = allpoints[4];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[5];
        points[2] = allpoints[6];
        points[3] = allpoints[7];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[8];
        points[2] = allpoints[9];
        points[3] = allpoints[10];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);
    }
    else
    {
        SetLength(points, 3);
        //中间点
        points[0] = MakePointF(left+index /2.0, top + index / 2.0);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[1];
        points[2] = allpoints[2];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[4];
        points[2] = allpoints[5];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[7];
        points[2] = allpoints[8];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);

        graphicsPath = TGPGraphicsPath.Create();
        points[1] = allpoints[10];
        points[2] = allpoints[11];
        graphicsPath.AddPolygon(PGPPointF(points), Length(points));
        tmpgraphicsPath.AddPolygon(PGPPointF(points), Length(points));
        Result.Add(graphicsPath);
    }
    tmpgraphicsPath.GetBounds(rectangleF);
    FreeAndNil(tmpgraphicsPath);
}
//4个交叉圆
DB_Action_DrawFourCirclePath(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    Double left;
    Double top;
    TGPPointFDynArray points;
    TGPGraphicsPath graphicsPath;
    TGPGraphicsPath tmpgraphicsPath;
    Result = TListGPGraphicsPath.Create;
    tmpgraphicsPath = TGPGraphicsPath.Create;
    left = (MaxD - index) / 2.0;
    top = left;


    graphicsPath = TGPGraphicsPath.Create();
    graphicsPath.AddArc(left-index/2.0, top,
                        index, index,270,180);
    tmpgraphicsPath.AddPath(graphicsPath,true);
    Result.Add(graphicsPath);

    graphicsPath = TGPGraphicsPath.Create();
    graphicsPath.AddArc(left, top-index/2.0,
                        index, index,0,180);
    tmpgraphicsPath.AddPath(graphicsPath,true);
    Result.Add(graphicsPath);


    graphicsPath = TGPGraphicsPath.Create();
    graphicsPath.AddArc(left+index/2.0, top,
                        index, index,90,180);
    tmpgraphicsPath.AddPath(graphicsPath,true);
    Result.Add(graphicsPath);

    graphicsPath = TGPGraphicsPath.Create();
    graphicsPath.AddArc(left, top+index/2.0,
                        index, index,180,180);
    tmpgraphicsPath.AddPath(graphicsPath,true);
    Result.Add(graphicsPath);


    tmpgraphicsPath.GetBounds(rectangleF);
    FreeAndNil(tmpgraphicsPath);

}
//音乐符号
DB_Action_DrawMusicPath(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    Double left;
    Double top;
    TGPPointFDynArray points;
    TGPGraphicsPath graphicsPath;
    Result = TListGPGraphicsPath.Create;

    left = (MaxD - index) / 2.0;
    top = left;

    //先算出几个用到的点
    SetLength(points,4);
    points[0] = MakePointF(left+0.361*index,top+(bitmap,index-0.129*index));
    points[1] = MakePointF(left+0.361*index,top+(bitmap,index-0.75*index));
    points[2] = MakePointF(left+0.84*index,top);
    points[3] = MakePointF(left+0.84*index,top+(bitmap,index-0.361*index));

    graphicsPath = TGPGraphicsPath.Create();
    //画直线
    //0->1
    graphicsPath.AddLine(points[0],points[1]);
    //1->2
    graphicsPath.AddLine(points[1],points[2]);
    //2->3
    graphicsPath.AddLine(points[2],points[3]);

    //画圆
    graphicsPath.AddEllipse(points[0].x-0.129*index*2,points[0].y-0.129*index,
                            0.129*index*2,0.129*index*2);
    graphicsPath.AddEllipse(points[3].x-0.129*index*2,points[3].y-0.129*index,
                            0.129*index*2,0.129*index*2);


    Result.Add(graphicsPath);
    graphicsPath.GetBounds(rectangleF);


}
//三角形
DB_Action_DrawTrianglePath(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    Double left;
    Double top;
    TGPPointFDynArray points;
    TGPGraphicsPath graphicsPath;
    Result = TListGPGraphicsPath.Create;

    left = (MaxD - index) / 2.0;
    top = left;
    //先算出几个用到的点
    SetLength(points,3);
    points[0] = MakePointF(left,top+index);
    points[1] = MakePointF(left+0.5*index,top+(bitmap,index-0.866*index));
    points[2] = MakePointF(left+index,top+index);

    graphicsPath = TGPGraphicsPath.Create();
    //三角形，画直线
    graphicsPath.AddPolygon(PGPPointF(points),3);



    Result.Add(graphicsPath);

    graphicsPath.GetBounds(rectangleF);

}
//六角星
DB_Action_DrawSixStarPath(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    Double left;
    Double top;
    TGPPointFDynArray points;
    TGPGraphicsPath graphicsPath;
    TGPGraphicsPath tmpgraphicsPath;
    Result = TListGPGraphicsPath.Create;
    tmpgraphicsPath = TGPGraphicsPath.Create;
    left = (MaxD - index) / 2.0;
    top = left;
    //先算出几个用到的点
    SetLength(points,6);
    points[0] = MakePointF(left+0.5*index,top);
    points[1] = MakePointF(left+index,top+0.25*index);
    points[2] = MakePointF(left+index,top+0.75*index);
    points[3] = MakePointF(left+0.5*index,top+index);
    points[4] = MakePointF(left,top+0.75*index);
    points[5] = MakePointF(left,top+0.25*index);

    //画2个三角形
    graphicsPath = TGPGraphicsPath.Create();
    //第一个 ,0,2,4
    //0->2
    graphicsPath.AddLine(points[0],points[2]);
    //2->4
    graphicsPath.AddLine(points[2],points[4]);
    //4->0
    graphicsPath.AddLine(points[4],points[0]);

    Result.Add(graphicsPath);
    tmpgraphicsPath.AddPath(graphicsPath,true);
    //第二个,1,3,5
    graphicsPath = TGPGraphicsPath.Create();
    //1->3
    graphicsPath.AddLine(points[1],points[3]);
    //3->5
    graphicsPath.AddLine(points[3],points[5]);
    //5->1
    graphicsPath.AddLine(points[5],points[1]);

    Result.Add(graphicsPath);
    tmpgraphicsPath.AddPath(graphicsPath,true);

    tmpgraphicsPath.GetBounds(rectangleF);
    FreeAndNil(tmpgraphicsPath);
}
//立方体
DB_Action_DrawCubePath(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    Double left;
    Double top;
    TGPPointFDynArray points;
    TGPPointFDynArray tmppoints;
    TGPGraphicsPath graphicsPath;
    TGPGraphicsPath tmpgraphicsPath;
    Result = TListGPGraphicsPath.Create;
    tmpgraphicsPath = TGPGraphicsPath.Create;
    index = index / 2 *2;
    left = (MaxD - index) / 2.0;
    top = left;
    //先算出几个用到的点
    SetLength(points,7);
    //正面4个点
    points[0] = MakePointF(left,top+index);
    points[1] = MakePointF(left,top+0.2*index);
    points[2] = MakePointF(left+0.8*index,top+0.2*index);
    points[3] = MakePointF(left+0.8*index,top+index);

    points[4] = MakePointF(left+index,top+0.8*index);
    points[5] = MakePointF(left+index,top);
    points[6] = MakePointF(left+0.2*index,top);

    //画3个四边形
    graphicsPath = TGPGraphicsPath.Create();
    //第一个 ,正面,0,1,2,3
    graphicsPath.AddPolygon(PGPPointF(points),4);
    Result.Add(graphicsPath);
    tmpgraphicsPath.AddPath(graphicsPath,true);


    SetLength(tmppoints,4);
    //第二个，侧面,2,3,4,5
    graphicsPath = TGPGraphicsPath.Create();
    tmppoints[0] = points[2];
    tmppoints[1] = points[3];
    tmppoints[2] = points[4];
    tmppoints[3] = points[5];
    graphicsPath.AddPolygon(PGPPointF(tmppoints),4);
    Result.Add(graphicsPath);
    tmpgraphicsPath.AddPath(graphicsPath,true);

    //第三个,顶面,1,2,5,6
    graphicsPath = TGPGraphicsPath.Create();

    tmppoints[0] = points[1];
    tmppoints[1] = points[2];
    tmppoints[2] = points[5];
    tmppoints[3] = points[6];
    graphicsPath.AddPolygon(PGPPointF(tmppoints),4);
    Result.Add(graphicsPath);
    tmpgraphicsPath.AddPath(graphicsPath,true);


    tmpgraphicsPath.GetBounds(rectangleF);
    FreeAndNil(tmpgraphicsPath);
}

//circle
DB_Action_DrawCircle(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    TGPGraphicsPath graphicsPath;
    graphicsPath = TGPGraphicsPath.Create();
    index = index / 2 *2;
    graphicsPath.AddEllipse((MaxD - index) / 2.0, (MaxD - index) / 2.0,
                            index, index);
    Result = TListGPGraphicsPath.Create;
    Result.Add(graphicsPath);
    graphicsPath.GetBounds(rectangleF);
}
//block
DB_Action_DrawBlock(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;

{
    TGPGraphicsPath graphicsPath;
    TGPPointFDynArray points;
    Double left;
    Double top;
    graphicsPath = TGPGraphicsPath.Create();
    index = index / 2 *2;
    left = RealToInt((MaxD - index) / 2.0);
    top = left;
    SetLength(points,4);
    points[0] = MakePointF(left, top);
    points[1] = MakePointF(left + index, top);
    points[2] = MakePointF(left + index, top + index);
    points[3] = MakePointF(left, top + index);

    graphicsPath.AddPolygon(PGPPointF(points),Length(points));
    Result = TListGPGraphicsPath.Create;
    Result.Add(graphicsPath);
    graphicsPath.GetBounds(rectangleF);

}
//Diamond
DB_Action_DrawDiamond(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    int left;
    int top;
    TGPGraphicsPath graphicsPath;
    TGPPointFDynArray points;
    graphicsPath = TGPGraphicsPath.Create();
    index = index / 2 *2;
    left = RealToInt((MaxD - index) / 2.0);
    top = left;
    // 菱形。
    SetLength(points,4);
    points[0] = MakePointF(left, RealToInt(top+0.5*index));
    points[1] = MakePointF(RealToInt(left+0.5*index), top);
    points[2] = MakePointF(left + index, RealToInt(top+0.5*index));
    points[3] = MakePointF(RealToInt(left+0.5*index), top + index);
    graphicsPath.AddPolygon(PGPPointF(points),Length(points));
    Result = TListGPGraphicsPath.Create;
    Result.Add(graphicsPath);
    graphicsPath.GetBounds(rectangleF);

}
//five star
DB_Action_DrawFiveStar(LedBitmap *bitmap, int index, int MaxD, LBM_RectF *prectangleF,KGColor color)
    : TListGPGraphicsPath;
{
    array[0..9] of TGPPoint pts;
    TPoint pt;
    int r;
    float rx;
    float radian;
    int i;
    int num;
    TGPGraphicsPath graphicsPath;

    // 创建五角星路径。
    // 五角星半径。
    index = index / 2 *2;
    r = index / 2;
    pt.X = RealToInt((MaxD - index) / 2.0+0.5*index);
    pt.Y = pt.X;
    for(i= 0; i<=9; i++)
    {
        rx = r;
        if Odd(i)  rx = r * (1- (Sqrt(5)-1)/2);
        radian = i * (360/10) * (Pi/180);
        pts[i].X = pt.X + Round(Sin(radian) * rx);
        pts[i].Y = pt.Y - Round(Cos(radian) * rx);
    }

    graphicsPath = TGPGraphicsPath.Create(FillModeWinding);
    graphicsPath.AddPolygon(PGPPoint(@pts), Length(pts));
    Result = TListGPGraphicsPath.Create;
    Result.Add(graphicsPath);
    graphicsPath.GetBounds(rectangleF);
}
*/
