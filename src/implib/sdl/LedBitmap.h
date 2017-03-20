#ifndef LEDBITMAP_H_INCLUDED
#define LEDBITMAP_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include "SDL.h"
//#include "SDL_video.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"
//#include "SDL_ttf.h"
#include "KGMarco.h"
#include "Common.h"

#define PI 3.1415926535897932385
#define LBM_RealTransparentColor 0x000000FF

#ifndef NOMINMAX
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#endif

typedef enum _BitBltMode
{
    BBM_SRCINVERT=0,
    BBM_SRCAND,
    BBM_SRCCOPY
}BitBltMode;

typedef enum _RotateFlipType
{
    Rotate90FlipNone = 0,
    Rotate270FlipNone,
    Rotate180FlipY,
    Rotate180FlipX
}RotateFlipType;
typedef  SDL_Surface LedBitmap;
typedef struct LBM_Point
{
    Sint16 X;
    Sint16 Y;
}LBM_Point;

typedef struct LBM_PointF
{
    float X;
    float Y;
}LBM_PointF;

typedef struct _LBM_RectF
{
    float X;
    float Y;
    float Width;
    float Height;
}LBM_RectF;

typedef struct RECTANGULARPARADCB
{
    //
    Sint16 Height;
    //
    Sint16 Width;
    // X
    Sint16 X;
    // Y
    Sint16 Y;
}RECTANGULARPARADCB;

Uint32 LBM_KGColorToSdlColor(KGColor kgcolor);
SDL_Color LBM_KGColorToSdlColorA(KGColor kgcolor);
int LBM_GetBitmapBits(KGPicType pictype);
LBM_PointF LBM_MakePointF(float x,float y);
LBM_Point LBM_MakePoint(Sint16 x,Sint16 y);
SDL_Rect LBM_MakeRect(int x,int y,int width,int height);
//void localeToUTF8(char *src,char *buf);

LedBitmap* LBM_CreateLedBitmap(int width,int height,KGPicType pictype);
int LBM_SaveLedBitmapToFile(LedBitmap *bitmap,const char* file, KGBitmap *bmpdata,KGCreateMode createMode);
void LBM_RotateLedBitmapDirect(LedBitmap *bitmap, float angle);
void LBM_ClearLedBitmap(LedBitmap *bitmap);
void LBM_FillLedBitmap(LedBitmap *bitmap,KGColor color);
LedBitmap* LBM_RotateLedBitmap(LedBitmap *bitmap, float angle);
void LBM_FreeLedBitmap(LedBitmap *bitmap);
void LBM_CopyLebBitmap(LedBitmap *sbitmap,RECTANGULARPARADCB srect,LedBitmap *obitmap,RECTANGULARPARADCB orect);
void LBM_GetZoomSize(int width, int height, double zoom, int *dstwidth, int *dstheight);
void LBM_GetRotoZoomSize(int width, int height, double angle, double zoom, int *dstwidth, int *dstheight);
LedBitmap* LBM_DrawFont(char *text,char *fontFile,int fontSize,KGColor color,
                        unsigned char bold,unsigned char italic,unsigned char underline);
LedBitmap *LBM_Clone(LedBitmap *bitmap);
LedBitmap *LBM_RotateFlip(LedBitmap *bitmap,RotateFlipType flipType);
void LBM_DrawImage(LedBitmap *bitmap,LedBitmap *srcBitmap,SDL_Rect destRect,int srcx, int srcy, int srcwidth, int srcheight);
void LBM_Draw(LedBitmap *bitmap,int x, int y,LedBitmap *srcBitmap);
void LBM_FillEllipse(LedBitmap *bitmap, SDL_Rect rect,KGColor color);
LedBitmap *LBM_Rotation(LedBitmap *bitmap,double angle);

#ifdef __cplusplus
}
#endif

#endif // LEDBITMAP_H_INCLUDED
