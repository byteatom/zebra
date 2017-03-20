#ifndef TILEEFFECTBASE_H_INCLUDED
#define TILEEFFECTBASE_H_INCLUDED



//
typedef enum _TileZoomMode
{
//
    tzmOut = 0,
//
    tzmIn,
//
    tzmOutIn
}TileZoomMode;

typedef struct _TileBaseParams
{
//
    int MaxD;
//
    int FColumnCount;
//
    int FRowCount;
//
    TileZoomMode FZoomMode;
}TileBaseParams;

typedef void (*pFGetGraphicsPath)(TileBaseParams *tileParams, LedBitmap *bitmap, int index, LBM_RectF *prectangleF,KGColor color);
typedef LedBitmap* (*pFGetGraphicsBitmap)(TileBaseParams *tileParams, int index, LBM_RectF *prectangleF, int *pelementIndex, KGPicType pictype, pFGetGraphicsPath GetGraphicsPath);


#endif // TILEEFFECTBASE_H_INCLUDED
