#include "DynamicBase.h"

int DB_Base_GetRandom(int max)
{
    return getRandom(max);
}

ShineElementTable *DB_Base_ShineElementTableAt(ShineElementTable *elements,int index)
{
    int i = 0;
    ShineElementTable *pnext = elements;
    if(index==0)
    {
        if(elements->isnull)
        {
            return NULL;
        }
        else
        {
            return elements;
        }
    }
    while(true)
    {
        if(index==i)
        {
            return pnext;
        }
        pnext = pnext->next;
        i++;
        if(pnext==NULL)
        {
            break;
        }
    }
    return NULL;
}
ShineElementTable *DB_Base_ShineElementTableAdd(ShineElementTable *elements)
{
    ShineElementTable *pnext = elements;

    if(pnext->isnull)
    {

        memset(pnext,0,sizeof(ShineElementTable));
        pnext->locusTable.isnull = true;
        pnext->locusTable.next = NULL;
        pnext->isnull = false;
        pnext->next = NULL;

    }
    else
    {
        while(1)
        {
            if(pnext->next==NULL)
            {
                pnext->next = (ShineElementTable *)malloc(sizeof(ShineElementTable));
                pnext = pnext->next;
				
                memset(pnext,0,sizeof(ShineElementTable));
                pnext->isnull = true;
                pnext->locusTable.isnull = true;
                pnext->locusTable.next = NULL;
                pnext->next = NULL;
                break;

            }
            pnext = pnext->next;
        }
    }

    return pnext;
}
int DB_Base_ShineElementTableCount(ShineElementTable *elements)
{
    int count = 0;
    ShineElementTable *pnext = elements;
    if(pnext->isnull)
    {

    }
    else
    {
        count = 1;
        while(pnext->next!=NULL)
        {
            count++;
            pnext = pnext->next;
        }
    }
	
    return count;
}
void DB_Base_ShineElementTableFree(ShineElementTable *elements)
{
    ShineElementTable *pnext = elements,*plast;
    if(elements->isnull)
    {
        return;
    }
	pnext = pnext->next;
    while(pnext!=NULL)
    {
        plast = pnext;
        pnext = pnext->next;
		
        free(plast);
        if(pnext==NULL)
        {
            break;
        }
    }
}
ShineLocusTable *DB_Base_ShineLocusTableAt(ShineLocusTable *locuss,int index)
{
    int i = 0;
    ShineLocusTable *pnext = locuss;
    if(index==0)
    {
        if(locuss->isnull)
        {
            return NULL;
        }
        else
        {
            return locuss;
        }
    }
    while(true)
    {
        if(index==i)
        {
            return pnext;
        }
        pnext = pnext->next;
        i++;
        if(pnext==NULL)
        {
            break;
        }
    }
    return NULL;
}
ShineLocusTable *DB_Base_ShineLocusTableAdd(ShineLocusTable *locuss)
{
    ShineLocusTable *pnext = locuss;
    if(pnext->isnull)
    {
        memset(pnext,0,sizeof(ShineLocusTable));
        pnext->isnull = false;
        pnext->next = NULL;

    }
    else
    {
        while(1)
        {
            if(pnext->next==NULL)
            {
                pnext->next = (ShineLocusTable *)malloc(sizeof(ShineLocusTable));
                pnext = pnext->next;
                memset(pnext,0,sizeof(ShineLocusTable));
                pnext->isnull = true;
                pnext->next = NULL;
                break;

            }
            pnext = pnext->next;
        }
    }
    pnext->nextPic = 0;
    pnext->offsetX = 0;
    pnext->offsetY = 0;
    pnext->pictureNumber = 0;
    pnext->recycleTotal = 0;
    return pnext;
}
int DB_Base_ShineLocusTableCount(ShineLocusTable *locuss)
{
    int count = 0;
    ShineLocusTable *pnext = locuss;
    if(pnext->isnull)
    {

    }
    else
    {
        count = 1;
        while(pnext->next!=NULL)
        {
            count++;
            pnext = pnext->next;
        }
    }
	
    return count;
}
void DB_Base_ShineLocusTableFree(ShineLocusTable *locuss)
{
    ShineLocusTable *pnext = locuss,*plast;
    if(locuss->isnull)
    {
        return;
    }
	pnext = pnext->next;
	while(pnext!=NULL)
	{
		plast = pnext;
		pnext = pnext->next;

		free(plast);
		if(pnext==NULL)
		{
			break;
		}
	}
}
ShinePictureTable *DB_Base_ShinePictureTableAt(ShinePictureTable *bitmaps,int index)
{
    int i = 0;
    ShinePictureTable *pnext = bitmaps;
    if(index==0)
    {
        if(bitmaps->isnull)
        {
            return NULL;
        }
        else
        {
            return bitmaps;
        }
    }
    while(true)
    {
        if(index==i)
        {
            return pnext;
        }
        pnext = pnext->next;
        i++;
        if(pnext==NULL)
        {
            break;
        }
    }
    return NULL;
}
ShinePictureTable *DB_Base_ShinePictureTableAdd(ShinePictureTable *bitmaps)
{
    ShinePictureTable *pnext = bitmaps;
    if(pnext->isnull)
    {
        memset(pnext,0,sizeof(ShinePictureTable));
        pnext->isnull = false;
        pnext->next = NULL;


        pnext->index = 0;
    }
    else
    {
        while(1)
        {
            if(pnext->next==NULL)
            {
                pnext->next = (ShinePictureTable *)malloc(sizeof(ShinePictureTable));
                memset(pnext->next,0,sizeof(ShinePictureTable));
                pnext->next->index = pnext->index+1;
                pnext = pnext->next;
                pnext->isnull = true;
                pnext->next = NULL;
                break;

            }
            pnext = pnext->next;
        }
    }
    return pnext;
}
int DB_Base_ShinePictureTableCount(ShinePictureTable *bitmaps)
{
    int count = 0;
    ShinePictureTable *pnext = bitmaps;
    if(pnext->isnull)
    {

    }
    else
    {
        count = 1;
        while(pnext->next!=NULL)
        {
            count++;
            pnext = pnext->next;
        }
    }
    return count;
}
void DB_Base_ShinePictureTableFree(ShinePictureTable *bitmaps)
{
    ShinePictureTable *pnext = bitmaps,*plast;
    if(bitmaps->isnull)
    {
        return;
    }
	pnext = pnext->next;
	while(pnext!=NULL)
	{
		plast = pnext;
		pnext = pnext->next;

		free(plast);
		if(pnext==NULL)
		{
			break;
		}
	}
}
int DB_Base_BoxingBitmap(ShinePictureTable *bitmaps,const char *dir,LedBitmap *bitmap,KGCreateMode createMode)
{
    char fn[1024] = "";
    //Bitmaps.Add(_bitmap);
    //Icoparadcbs.Add(ICOPARADCB.ICOPARADCB(_bitmap.Width,_bitmap.Height));
    ShinePictureTable *pic = DB_Base_ShinePictureTableAdd(bitmaps);
    pic->width = bitmap->w;
    pic->height = bitmap->h;
    strcpy(pic->pictureContent,dir);
    sprintf(fn,"%u%d_%d",getCurrentTime(),DB_Base_GetRandom(0),pic->index);
    strcat(pic->pictureContent,fn);
    strcat(pic->pictureContent,".bmp");
    return LBM_SaveLedBitmapToFile(bitmap,pic->pictureContent,&(pic->bitmap),createMode);


}
LedBitmap *DB_Base_Zoom(LedBitmap *bitmap, int valueWidth, int valueHeight)
{
    int originWidth;
    int originHeight;
    LedBitmap *zoomBitmap;

    int zoomWidth;
    int zoomHeight;
    double zoomx,zoomy;
    if (bitmap == NULL)
    {
        return NULL;
    }
    originWidth = bitmap->w;
    originHeight = bitmap->h;

    zoomWidth = originWidth + valueWidth;
    zoomHeight = originHeight + valueHeight;
    zoomx = (double)zoomWidth/(double)bitmap->w;
    zoomy = (double)zoomHeight/(double)bitmap->h;
    zoomBitmap = rotozoomSurfaceXY(bitmap,0,zoomx, zoomy,0);

    return zoomBitmap;
}
void DB_Base_FillCustumColor(LedBitmap *bmpDest, SDL_Rect rcDest,KGColor color, LedBitmap *bmpColor, bool bTile)
{
    LedBitmap *bmpmask;
    LedBitmap *bmpblack;

    SDL_Rect dstrect;
    int x,y;
    //bmpDest.Canvas.Draw(0,0,bmpColor);
    //Exit;
    bmpmask = LBM_CreateLedBitmap(rcDest.w,rcDest.h,32);

    if( bmpColor!=NULL)
    {

        if( bTile)
        {

            x = 0;
            y = 0;
            while(true)
            {
                dstrect.x = x;
                dstrect.y = y;
                dstrect.w = bmpColor->w;
                dstrect.h = bmpColor->h;
                SDL_BlitSurface(bmpColor, NULL, bmpmask, &dstrect);
                x += bmpColor->w;
                if(x>=bmpmask->w)
                {
                    y += bmpColor->h;
                    if(y>=bmpmask->h)
                    {
                        break;
                    }
                    x = 0;
                }

            }
        }
        else
        {
            LedBitmap *bmpZoom;
            dstrect.x = 0;
            dstrect.y = 0;
            dstrect.w = bmpmask->w;
            dstrect.h = bmpmask->h;
            //SDL_BlitSurface(bmpColor, NULL, bmpmask, &dstrect);
            //LBM_SaveLedBitmapToFile(bmpColor,"e:/temp/led/color.bmp",NULL,0);
            //SDL_SoftStretch(bmpColor, &(bmpColor->clip_rect), bmpmask, &dstrect);

            //bmpmask = SDL_ScaleSurface(bmpColor,dstrect.w,dstrect.h);
            bmpZoom = zoomSurface(bmpColor,dstrect.w/bmpColor->w,dstrect.h/bmpColor->h,1);
            SDL_BlitSurface(bmpZoom, NULL, bmpmask, &dstrect);
            LBM_FreeLedBitmap(bmpZoom);
        }

    }
    else
    {
        SDL_Color cl = LBM_KGColorToSdlColorA(color);
        SDL_FillRect( bmpmask, &bmpmask->clip_rect, SDL_MapRGBA( bmpmask->format, cl.r, cl.g, cl.b ,cl.unused) );
    }
    //LBM_SaveLedBitmapToFile(bmpmask,"e:/temp/led/mask.bmp",NULL,0);
    bmpblack = LBM_CreateLedBitmap(bmpmask->w,bmpmask->h,32);

    LBM_BitBlt(bmpmask,0,0,bmpmask->w,bmpmask->h,
               bmpblack,0,0,BBM_SRCINVERT);
    //LBM_SaveLedBitmapToFile(bmpmask,"e:/temp/led/mask1.bmp",NULL,0);

    LBM_BitBlt(bmpDest,rcDest.x,rcDest.y,bmpmask->w,bmpmask->h,
               bmpmask,0,0,BBM_SRCAND);
    LBM_FreeLedBitmap(bmpblack);
    LBM_FreeLedBitmap(bmpmask);

}
