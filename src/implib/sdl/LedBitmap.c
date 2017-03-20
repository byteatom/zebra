#include "LedBitmap.h"
#include "SDL_stdinc.h"
#include "SDL_ttf.h"

/* Compression encodings for BMP files */
#ifndef BI_RGB
#define BI_RGB		0
#define BI_RLE8		1
#define BI_RLE4		2
#define BI_BITFIELDS	3
#endif

//KGColor
Uint32 LBM_KGColorToSdlColor(KGColor kgcolor)
{
    Uint32 color = 0x000000FF;
    switch(kgcolor)
    {
    case KG_COLOR_RED:
        color = 0xFF0000FF;
        break;
    case KG_COLOR_GREEN:
        color = 0x00FF00FF;
        break;
    case KG_COLOR_BLUE:
        color = 0x0000FFFF;
        break;
    case KG_COLOR_YELLOW:
        color = 0xFFFF00FF;
        break;
    case KG_COLOR_MAGENTA:
        color = 0xFF00FFFF;
        break;
    case KG_COLOR_CYAN:
        color = 0x00FFFFFF;
        break;
    case KG_COLOR_WHITE:
        color = 0xFFFFFFFF;
        break;
    case KG_COLOR_BLACK:
        color = 0x000000FF;
        break;
    case KG_COLOR_PURPLE:
        color = 0x800080FF;
        break;
    case KG_COLOR_PINK:
        color = 0xFFC0CBFF;
        break;
    case KG_COLOR_PANSY:
        color = 0x7400A1FF;
        break;
    case KG_COLOR_ORANGE:
        color = 0xFFA500FF;
        break;
    }
    return color;
}
SDL_Color LBM_KGColorToSdlColorA(KGColor kgcolor)
{
    SDL_Color color = {0x00,0x00,0x00,0xFF};
    Uint32 cl = LBM_KGColorToSdlColor(kgcolor);
    color.r = cl>>24 & 0xFF;
    color.g = cl>>16 & 0xFF;
    color.b = cl>>8 &0xFF;
    color.unused = cl &0xFF;
    return color;
}
//
int LBM_GetBitmapBits(KGPicType pictype)
{
    switch(pictype)
    {
    case KG_BMP_1BIT:
        return 16;
    case KG_BMP_32BIT:
        return 32;
    case KG_BMP_24BIT:
    case KG_PNG_24BIT:
        return 24;

    }
    return 24;
}

/*
void localeToUTF8(char *src,char *buf)
{

    wchar_t *unicode_buf;
    int nRetLen = MultiByteToWideChar(CP_ACP,0,src,-1,NULL,0);
    unicode_buf = (wchar_t*)malloc((nRetLen+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP,0,src,-1,unicode_buf,nRetLen);
    nRetLen = WideCharToMultiByte(CP_UTF8,0,unicode_buf,-1,NULL,0,NULL,NULL);

    WideCharToMultiByte(CP_UTF8,0,unicode_buf,-1,buf,nRetLen,NULL,NULL);
    free(unicode_buf);
    return buf;
}
*/
//point
LBM_PointF LBM_MakePointF(float x,float y)
{
    LBM_PointF point;
    point.X = x;
    point.Y = y;
    return point;
}
LBM_Point LBM_MakePoint(Sint16 x,Sint16 y)
{
    LBM_Point point;
    point.X = x;
    point.Y = y;
    return point;
}
//
void LBM_Init()
{
    SDL_Init(0);//SDL_INIT_EVERYTHING);
}
//
void LBM_Free()
{
    SDL_Quit();
}
//
LedBitmap* LBM_CreateLedBitmap(int width,int height,KGPicType pictype)
{
    Uint32 rmask, gmask, bmask, amask;
    LedBitmap *screen = NULL;
    int depth = 32;//LBM_GetBitmapBits(pictype);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    rmask = 0x00;
    gmask = 0x00;
    bmask = 0x00;
    amask = 0xff;

    //rmask = 0xFF000000; gmask = 0x00FF0000; bmask = 0x0000FF00; amask = 0x000000FF;   // RGBA8888模式
    rmask = 0x00FF0000;
    gmask = 0x0000FF00;
    bmask = 0x000000FF;
    amask = 0xFF000000;   // ARGB8888模式
    //rmask = 0xFF000000; gmask = 0x00FF0000; bmask = 0x0000FF00; amask = 0x00000000; // RGB8888模式
    //screen = SDL_SetVideoMode(480, 640, 32, SDL_SWSURFACE);//
    //default black background
    screen = SDL_CreateRGBSurface(0,width, height, depth,rmask, gmask, bmask, amask);
    //SDL_SetAlpha(screen,SDL_SRCALPHA,amask);
    //LBM_ClearLedBitmap(screen);
    //printf("%d,%d\n",screen->format->BitsPerPixel,screen->format->BytesPerPixel);
    //printf("LBM_CreateLedBitmap: %d\n",screen->format->alpha);
    return screen;
}
LedBitmap* LBM_CreateLedBitmapRGB(int width,int height,KGPicType pictype)
{
    Uint32 rmask, gmask, bmask, amask;
    LedBitmap *screen = NULL;
    int depth = 32;//LBM_GetBitmapBits(pictype);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    rmask = 0x00;
    gmask = 0x00;
    bmask = 0x00;
    amask = 0xff;

    //rmask = 0xFF000000; gmask = 0x00FF0000; bmask = 0x0000FF00; amask = 0x000000FF;   // RGBA8888模式
    //rmask = 0x00FF0000;    gmask = 0x0000FF00;    bmask = 0x000000FF;    amask = 0xFF000000;   // ARGB8888模式
    rmask = 0xFF000000; gmask = 0x00FF0000; bmask = 0x0000FF00; amask = 0x00000000; // RGB8888模式
    //screen = SDL_SetVideoMode(480, 640, 32, SDL_SWSURFACE);//
    //default black background
    screen = SDL_CreateRGBSurface(0,width, height, depth,rmask, gmask, bmask, amask);
    //SDL_SetAlpha(screen,SDL_SRCALPHA,amask);
    //LBM_ClearLedBitmap(screen);
    //printf("%d,%d\n",screen->format->BitsPerPixel,screen->format->BytesPerPixel);

    return screen;
}
LedBitmap* LBM_RotateLedBitmap(LedBitmap *bitmap, float angle)
{
    SDL_Rect srcrect,dstrect;
    LedBitmap *obitmap = LBM_CreateLedBitmap(bitmap->w,bitmap->h,bitmap->format->BitsPerPixel);
    LedBitmap *rbitmap = rotozoomSurface(bitmap,angle,1,0);
    srcrect.x = (rbitmap->w-bitmap->w)/2;
    srcrect.y = (rbitmap->h-bitmap->h)/2;
    srcrect.w = bitmap->w;
    srcrect.h = bitmap->h;

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = bitmap->w;
    dstrect.h = bitmap->h;
    SDL_BlitSurface(rbitmap, &srcrect, obitmap, &dstrect);
    LBM_FreeLedBitmap(rbitmap);
    return obitmap;
}
void LBM_ClearLedBitmap(LedBitmap *bitmap)
{
    if(bitmap!=NULL)
    {
        SDL_FillRect( bitmap, &bitmap->clip_rect, SDL_MapRGBA( bitmap->format, 0x00, 0x00, 0x00 ,0xFF) );
    }
}
void LBM_FillLedBitmap(LedBitmap *bitmap,KGColor color)
{
    if(bitmap!=NULL)
    {
        SDL_Color cl = LBM_KGColorToSdlColorA(color);
        SDL_FillRect( bitmap, &bitmap->clip_rect, SDL_MapRGBA( bitmap->format, cl.r, cl.g, cl.b ,cl.unused) );
    }
}
void LBM_CopyLebBitmap(LedBitmap *sbitmap,RECTANGULARPARADCB srect,LedBitmap *obitmap,RECTANGULARPARADCB orect)
{
    SDL_Rect srcrect,dstrect;
    srcrect.x = srect.X;
    srcrect.y = srect.Y;
    srcrect.w = srect.Width;
    srcrect.h = srect.Height;

    dstrect.x = orect.X;
    dstrect.y = orect.Y;
    dstrect.w = orect.Width;
    dstrect.h = orect.Height;
    SDL_BlitSurface(sbitmap, &srcrect, obitmap, &dstrect);

}
void LBM_RotateLedBitmapDirect(LedBitmap *bitmap, float angle)
{
    SDL_Rect srcrect,dstrect;

    LedBitmap *rbitmap = rotozoomSurface(bitmap,angle,1,0);
    srcrect.x = (rbitmap->w-bitmap->w)/2;
    srcrect.y = (rbitmap->h-bitmap->h)/2;
    srcrect.w = bitmap->w;
    srcrect.h = bitmap->h;

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = bitmap->w;
    dstrect.h = bitmap->h;
    SDL_BlitSurface(rbitmap, &srcrect, bitmap, &dstrect);

    LBM_FreeLedBitmap(rbitmap);

}
int SDL_SaveBMP_RWA (SDL_Surface *saveme, SDL_RWops *dst, int freedst)
{
    long fp_offset;
    int i, pad;
    SDL_Surface *surface;
    Uint8 *bits;

    /* The Win32 BMP file header (14 bytes) */
    char   magic[2] = { 'B', 'M' };
    Uint32 bfSize;
    Uint16 bfReserved1;
    Uint16 bfReserved2;
    Uint32 bfOffBits;

    /* The Win32 BITMAPINFOHEADER struct (40 bytes) */
    Uint32 biSize;
    Sint32 biWidth;
    Sint32 biHeight;
    Uint16 biPlanes;
    Uint16 biBitCount;
    Uint32 biCompression;
    Uint32 biSizeImage;
    Sint32 biXPelsPerMeter;
    Sint32 biYPelsPerMeter;
    Uint32 biClrUsed;
    Uint32 biClrImportant;

    /* Make sure we have somewhere to save */
    surface = NULL;
    if ( dst )
    {
        if ( saveme->format->palette )
        {
            if ( saveme->format->BitsPerPixel == 8 )
            {
                surface = saveme;
            }
            else
            {
                SDL_SetError("%d bpp BMP files not supported",
                             saveme->format->BitsPerPixel);
            }
        }
        else if ( (saveme->format->BitsPerPixel == 24) &&
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
                  (saveme->format->Rmask == 0x00FF0000) &&
                  (saveme->format->Gmask == 0x0000FF00) &&
                  (saveme->format->Bmask == 0x000000FF)
#else
                  (saveme->format->Rmask == 0x000000FF) &&
                  (saveme->format->Gmask == 0x0000FF00) &&
                  (saveme->format->Bmask == 0x00FF0000)
#endif
                )
        {
            surface = saveme;
        }
        else
        {
            SDL_Rect bounds;

            /* Convert to 24 bits per pixel */
            surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                           saveme->w, saveme->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
                                           0x00FF0000, 0x0000FF00, 0x000000FF,
#else
                                           0x000000FF, 0x0000FF00, 0x00FF0000,
#endif
                                           0);
            if ( surface != NULL )
            {
                bounds.x = 0;
                bounds.y = 0;
                bounds.w = saveme->w;
                bounds.h = saveme->h;
                if ( SDL_LowerBlit(saveme, &bounds, surface,
                                   &bounds) < 0 )
                {
                    SDL_FreeSurface(surface);
                    SDL_SetError(
                        "Couldn't convert image to 24 bpp");
                    surface = NULL;
                }
            }
        }
    }

    if ( surface && (SDL_LockSurface(surface) == 0) )
    {
        const int bw = surface->w*surface->format->BytesPerPixel;

        /* Set the BMP file header values */
        bfSize = 0;		 /* We'll write this when we're done */
        bfReserved1 = 0;
        bfReserved2 = 0;
        bfOffBits = 0;		/* We'll write this when we're done */

        /* Write the BMP file header values */
        fp_offset = SDL_RWtell(dst);
        SDL_ClearError();
        SDL_RWwrite(dst, magic, 2, 1);
        SDL_WriteLE32(dst, bfSize);
        SDL_WriteLE16(dst, bfReserved1);
        SDL_WriteLE16(dst, bfReserved2);
        SDL_WriteLE32(dst, bfOffBits);

        /* Set the BMP info values */
        biSize = 40;
        biWidth = surface->w;
        biHeight = surface->h;
        biPlanes = 1;
        biBitCount = surface->format->BitsPerPixel;
        biCompression = BI_RGB;
        biSizeImage = surface->h*surface->pitch;
        biXPelsPerMeter = 0;
        biYPelsPerMeter = 0;
        if ( surface->format->palette )
        {
            biClrUsed = surface->format->palette->ncolors;
        }
        else
        {
            biClrUsed = 0;
        }
        biClrImportant = 0;

        /* Write the BMP info values */
        SDL_WriteLE32(dst, biSize);
        SDL_WriteLE32(dst, biWidth);
        SDL_WriteLE32(dst, biHeight);
        SDL_WriteLE16(dst, biPlanes);
        SDL_WriteLE16(dst, biBitCount);
        SDL_WriteLE32(dst, biCompression);
        SDL_WriteLE32(dst, biSizeImage);
        SDL_WriteLE32(dst, biXPelsPerMeter);
        SDL_WriteLE32(dst, biYPelsPerMeter);
        SDL_WriteLE32(dst, biClrUsed);
        SDL_WriteLE32(dst, biClrImportant);

        /* Write the palette (in BGR color order) */
        if ( surface->format->palette )
        {
            SDL_Color *colors;
            int       ncolors;

            colors = surface->format->palette->colors;
            ncolors = surface->format->palette->ncolors;
            for ( i=0; i<ncolors; ++i )
            {
                SDL_RWwrite(dst, &colors[i].b, 1, 1);
                SDL_RWwrite(dst, &colors[i].g, 1, 1);
                SDL_RWwrite(dst, &colors[i].r, 1, 1);
                SDL_RWwrite(dst, &colors[i].unused, 1, 1);
            }
        }

        /* Write the bitmap offset */
        bfOffBits = SDL_RWtell(dst)-fp_offset;
        if ( SDL_RWseek(dst, fp_offset+10, RW_SEEK_SET) < 0 )
        {
            SDL_Error(SDL_EFSEEK);
        }
        SDL_WriteLE32(dst, bfOffBits);
        if ( SDL_RWseek(dst, fp_offset+bfOffBits, RW_SEEK_SET) < 0 )
        {
            SDL_Error(SDL_EFSEEK);
        }

        /* Write the bitmap image upside down */
        bits = (Uint8 *)surface->pixels+(surface->h*surface->pitch);
        pad  = ((bw%4) ? (4-(bw%4)) : 0);
        while ( bits > (Uint8 *)surface->pixels )
        {
            bits -= surface->pitch;
            if ( SDL_RWwrite(dst, bits, 1, bw) != bw)
            {
                SDL_Error(SDL_EFWRITE);
                break;
            }
            if ( pad )
            {
                const Uint8 padbyte = 0;
                for ( i=0; i<pad; ++i )
                {
                    SDL_RWwrite(dst, &padbyte, 1, 1);
                }
            }
        }

        /* Write the BMP file size */
        bfSize = SDL_RWtell(dst)-fp_offset;
        if ( SDL_RWseek(dst, fp_offset+2, RW_SEEK_SET) < 0 )
        {
            SDL_Error(SDL_EFSEEK);
        }
        SDL_WriteLE32(dst, bfSize);
        if ( SDL_RWseek(dst, fp_offset+bfSize, RW_SEEK_SET) < 0 )
        {
            SDL_Error(SDL_EFSEEK);
        }

        /* Close it up.. */
        SDL_UnlockSurface(surface);
        if ( surface != saveme )
        {
            SDL_FreeSurface(surface);
        }
    }

    if ( freedst && dst )
    {
        SDL_RWclose(dst);
    }
    return((SDL_strcmp(SDL_GetError(), "") == 0) ? 0 : -1);
}
int LBM_SaveLedBitmapToFile(LedBitmap *bitmap,const char* file, KGBitmap *bmpdata,KGCreateMode createMode)
{
    int result = -1;

    //printf("%d,%d\n",bitmap->format->BitsPerPixel,bitmap->format->BytesPerPixel);
    //result = SDL_SaveBMP(bitmap,file);
    if(createMode==KG_CREATE_FILE)
    {
        //result = SDL_SaveBMP_RWA(bitmap, SDL_RWFromFile(file, "wb"), 1);
        result = SDL_SaveBMP(bitmap,file);
    }
    else
    {
        bmpdata->format = Format_ARGB32;
        bmpdata->size = bitmap->w*bitmap->h*bitmap->format->BytesPerPixel;
        bmpdata->data = (unsigned char *)malloc(bmpdata->size);
        bmpdata->width = bitmap->w;
        bmpdata->height = bitmap->h;
        memcpy(bmpdata->data,bitmap->pixels,bmpdata->size);
        result = 0;
    }

    return result;
}
void LBM_FreeLedBitmap(LedBitmap *bitmap)
{
    if(bitmap!=NULL)
    {
        SDL_FreeSurface(bitmap);
    }


}
void LBM_GetZoomSize(int width, int height, double zoom, int *dstwidth, int *dstheight)
{
    zoomSurfaceSize(width, height, zoom, zoom, dstwidth, dstheight);
}
void LBM_GetRotoZoomSize(int width, int height, double angle, double zoom, int *dstwidth, int *dstheight)
{
    rotozoomSurfaceSize(width, height, angle, zoom, dstwidth, dstheight);
}

LedBitmap* LBM_DrawFont(char *text,char *fontFile,int fontSize,KGColor color,
                        unsigned char bold,unsigned char italic,unsigned char underline)
{
    SDL_Rect dstrect;
    LedBitmap *bitmap = NULL,*result = NULL;
    TTF_Font *font;
    int style = TTF_STYLE_NORMAL;
    //TTF_Init();
    font = TTF_OpenFont(fontFile, fontSize);

    if(!font)
    {
        //printf("TTF_OpenFont failed: %s\n", SDL_GetError());
        //MessageBox(0,0,"no",0);
        return NULL;
    }
    if(bold)
    {
        style |= TTF_STYLE_BOLD;
    }
    if(italic)
    {
        style |= TTF_STYLE_ITALIC;
    }
    if(underline)
    {
        style |= TTF_STYLE_UNDERLINE;
    }
    TTF_SetFontStyle(font,style);
    bitmap = TTF_RenderUTF8_Solid(font,text,LBM_KGColorToSdlColorA(color));
    if(bitmap!=NULL)
    {
        dstrect.x = 0;
        dstrect.y = 0;
        dstrect.w = bitmap->w;
        dstrect.h = bitmap->h;
        result = LBM_CreateLedBitmap(bitmap->w,bitmap->h,KG_BMP_32BIT);
        SDL_BlitSurface(bitmap, NULL, result, &dstrect);
        //	SDL_BlitSurface(text, NULL, screen, &dstrect);
        LBM_FreeLedBitmap(bitmap);
    }
    TTF_CloseFont(font);

    //TTF_Quit();
    return result;
}
Uint32 LBM_GetPixel(LedBitmap *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void LBM_SetPixel(LedBitmap *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
void LBM_BitBlt(LedBitmap *bmpDest,int xDest,int yDest,int widthDest,int heightDest,LedBitmap *bmpSrc,int xSrc,int ySrc,BitBltMode bbMode)
{
    int x,y;
    Sint32 pixelDest,pixelSrc;
    int width = min(widthDest,bmpSrc->w);
    int height = min(heightDest,bmpSrc->h);
    Uint8 r=0,g=0,b=0,a=0;
    for(x=0; x<width; x++)
    {
        for(y=0; y<height; y++)
        {
            pixelDest = LBM_GetPixel(bmpDest,x+xDest,y+yDest);
            SDL_GetRGBA(pixelDest,bmpDest->format,&r,&g,&b,&a);
            pixelSrc = LBM_GetPixel(bmpSrc,x+xSrc,y+ySrc);
            if(bbMode==BBM_SRCINVERT)
            {
                pixelDest = pixelDest ^ pixelSrc;
            }
            else if(bbMode==BBM_SRCAND)
            {
                pixelDest = pixelDest & pixelSrc;
            }
            else if(bbMode==BBM_SRCCOPY)
            {
                pixelDest = pixelSrc;
            }
            LBM_SetPixel(bmpDest,x+xDest,y+yDest,pixelDest);
        }
    }
}
//flip
LedBitmap *LBM_RotateFlip(LedBitmap *bitmap,RotateFlipType flipType)
{
    if(flipType==Rotate180FlipY)
    {
        return rotozoomSurfaceXY(bitmap,0,-1,1,0);
    }
    else if(flipType==Rotate180FlipX)
    {
        return rotozoomSurfaceXY(bitmap,0,1,-1,0);
    }
    else if(flipType==Rotate90FlipNone)
    {
        return rotateSurface90Degrees(bitmap,1);
    }
    else if(flipType==Rotate270FlipNone)
    {
        return rotateSurface90Degrees(bitmap,3);
    }
    else
    {
        return rotozoomSurfaceXY(bitmap,0,1,1,0);
    }
}
LedBitmap *LBM_Clone(LedBitmap *bitmap)
{
    return rotozoomSurfaceXY(bitmap,0,1,1,0);

}
SDL_Rect LBM_MakeRect(int x,int y,int width,int height)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    return r;
}
void LBM_DrawImage(LedBitmap *bitmap,LedBitmap *srcBitmap,SDL_Rect destRect,int srcx, int srcy, int srcwidth, int srcheight)
{

    SDL_Rect srcRect;

    SDL_PixelFormat fmt = *(bitmap->format);
    LedBitmap *tmpBmp;
    srcRect.x = srcx;
    srcRect.y = srcy;
    srcRect.w = srcwidth;
    srcRect.h = srcheight;

    fmt.Amask = 0;
    //tmpBmp = SDL_ConvertSurface(bitmap,&fmt,0);
    //SDL_BlitSurface(srcBitmap, &srcRect, tmpBmp, &destRect);
    //sprintf(fn,"e:/temp/led/%d.bmp",tmpBmp);
    //LBM_SaveLedBitmapToFile(tmpBmp,fn,NULL,0);
    //LBM_FreeLedBitmap(tmpBmp);

    //fmt.alpha = 0;
    //tmpBmp = SDL_ConvertSurface(srcBitmap,&fmt,0);
    //SDL_BlitSurface(tmpBmp, &srcRect, bitmap, &destRect);
    //LBM_FreeLedBitmap(tmpBmp);


    //SDL_SetColorKey( srcBitmap, SDL_SRCCOLORKEY, SDL_MapRGB( srcBitmap->format, 0, 0, 0 ) );
    SDL_SetAlpha(srcBitmap, 0, SDL_ALPHA_TRANSPARENT);
    SDL_BlitSurface(srcBitmap, &srcRect, bitmap, &destRect);
}
void LBM_Draw(LedBitmap *bitmap,int x, int y,LedBitmap *srcBitmap)
{
    SDL_Rect srcRect,destRect;
    SDL_PixelFormat fmt = *(bitmap->format);
    LedBitmap *tmpBmp;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = srcBitmap->w;
    srcRect.h = srcBitmap->h;

    destRect.x = x;
    destRect.y = y;
    destRect.w = srcBitmap->w;
    destRect.h = srcBitmap->h;

    fmt.Amask = 0;
    //tmpBmp = SDL_ConvertSurface(srcBitmap,&fmt,0);
    //SDL_BlitSurface(tmpBmp, &srcRect, bitmap, &destRect);
    //LBM_FreeLedBitmap(tmpBmp);

    SDL_SetAlpha(srcBitmap, 0, SDL_ALPHA_TRANSPARENT);
    SDL_BlitSurface(srcBitmap, &srcRect, bitmap, &destRect);
}

void LBM_FillEllipse(LedBitmap *bitmap, SDL_Rect rect,KGColor color)
{
    int x = rect.x+rect.w/2;
    int y = rect.y+rect.h/2;
    int rx = rect.w/2;
    int ry = rect.h/2;
    if(x+rx>=rect.w)
    {
        //rx = rect.w-x-1;
    }
    if(y+ry>=rect.h)
    {
        //ry = rect.h-y-1;
    }
    filledEllipseColor(bitmap,x,y,rx,ry,LBM_KGColorToSdlColor(color));
}
LedBitmap *LBM_Rotation(LedBitmap *bitmap,double angle)
{
    return rotozoomSurface(bitmap,angle,1,0);
}
