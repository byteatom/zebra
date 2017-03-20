#ifndef KGWATCHFUNCS_H_INCLUDED
#define KGWATCHFUNCS_H_INCLUDED

#include "LedBitmap.h"
#include "KGWatch.h"

typedef struct _DrawDialParaExt
{
    Sint32 _radius;
    Sint32 _radiusA;
    Sint32 _centerX;
    Sint32 _centerY;
    char fileNamePrefix[1024];
}DrawDialParaExt;

typedef enum _LedFileType
{
    LFT_Dial = 0,
    LFT_HandHour,
    LFT_HandMinute,
    LFT_HandSecond
}LedFileType;

void KGW_GetPolygon(float centerX, float centerY, float thickness, float length, Uint32 color, float radians, LBM_PointF points[]);
RECTANGULARPARADCB KGW_GetPolygonRectangle(float centerX, float centerY, float thickness, float length, Uint32 color, float radians);
void KGW_DrawPolygon(HandType handType,float centerX, float centerY, float thickness, float slength, Uint32 color, float radians, LedBitmap *bitmap, LBM_Point *pLeft);
void KGW_DrawTick(LedBitmap *Graphics, Uint32 color32, float centerX, float centerY,float _radius, float tickRadius, float offsetRaduis, float angle);
void KGW_DrawDot(LedBitmap *Graphics, Sint32 color32, float centerX, float centerY, float _radius, float radius,float angle);
void KGW_DrawThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt);

int drawDialPic(const WatchAreaInput *input, WatchAreaOutput *output, const char *path, KGPoint *centre, const DrawDialParaExt *dialParaExt);
void KGW_MakeThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt);
void KGW_MakeRoundRectThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt);
void KGW_MakeEllipseThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt);
void KGW_MakeRhombusThicks(LedBitmap *Graphics, const DrawDialPara *drawPara, const DrawDialParaExt *dialParaExt);

#endif // KGWATCHFUNCS_H_INCLUDED
