
#include "kaler_config.h"
#include "kaler_struct.h"
#include "file_ops.h"
#include "parse_screen.h"

#ifdef undprif
#undef dprif
#define dprif(fmt, ...)
#endif

/*
 *******************************************************************************
 * author:
 * date:
 * description: return 1 - little, return 0 - big
 *******************************************************************************
 */
// dprif("cpu mode : %d (0-big, 1-little)\n", CheckCPU());
int CheckCPU(void)
{
    union w
    {
        int  a;
        char b;
    } c;
    c.a = 1;
    return(c.b == 1);
}

/*
 *******************************************************************************
 * author:
 * date:
 * description: return 1 - little, return 0 - big
 *******************************************************************************
 */
int PrintXXXFile(void)
{
    int i, j;
    uint8 data[78];
    FILE *fp;
    char path[256];
    for (i = 1; i < 35; i++)
    {
        sprintf(path, "./xxx/%d.SCF", i);
        fp = fopen(path, "rb");
        fseek(fp, 0, SEEK_SET);
        fread(data, sizeof(char), 78, fp);

        printf("//scf:%d\n", i);
        for (j = 0; j < 78; j++)
        {
            if ((j%30 == 0) && (j != 0))
                printf("\n");
            printf("0x%.2X,", data[j]);
        }
        printf("\n\n");
        fclose(fp);
    }

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
uint32 GetFileSizeK(FILE* fp)
{
    uint32 cur;
    uint32 size;

    cur = ftell(fp);
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, cur, SEEK_SET);

    return size;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int DetachFilePath(const char *path, char *dir, char *file)
{
    int32 len, cnt;

    if (path == NULL)
        return -1;

    cnt = len = strlen(path);

    while (--cnt >= 0) {
        if (path[cnt] != '/') {
            continue;
        } else {
            break;
        }
    }

    if (cnt > 0) {
        if (dir != NULL) {
            memset(dir, 0, cnt+2);
            memcpy(dir, path, cnt+1);
        }
        if (file != NULL) {
            memset(file, 0, len-(cnt+1)+1);
            memcpy(file, path+cnt+1, len-(cnt+1));
        }
    } else if (cnt < 0) {
        if (dir != NULL)
            memset(dir, 0, len);
        if (file != NULL)
            memset(file, 0, len);
        return -2;
    } else {
        if (dir != NULL)
            memcpy(dir, "/", sizeof("/"));
        if (file != NULL) {
            memset(file, 0, len);
            memcpy(file, path+1, len-1);
        }
    }

    //dprif("%s\n%s\n", dir, file);
    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int InitJsonFileBuf(const char *path, char ** const buf)
{
    FILE* fp = NULL;
    uint32 filesize;
    uint32 cnt;

    assert(path);

    if ((fp = fopen(path, "r")) == NULL)
    {
        printf("fopen json file failed\n");
        return -1;
    }

    filesize = GetFileSizeK(fp);
    //dprif("file size %d\n", filesize);

    if ((*buf = malloc(filesize + 1)) == NULL)
    {
        fclose(fp);
        printf("malloc json buf failed\n");
        return -2;
    }

    fseek(fp, 0, SEEK_SET);
    cnt = fread(*buf, sizeof(char), filesize + 1, fp);
    (*buf)[cnt] = 0;
    //dprif("fread size %d\n", cnt);

    fclose(fp);

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
void FreeJsonFileBuf(char *buf)
{
    if (buf != NULL)
    {
        free(buf);
    }
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CreateDirectoryK(const char *dirpath)
{
    if (dirpath == NULL)
        return -1;

    char dir[512];
    int i, len;

    strcpy(dir, dirpath);
    len = strlen(dir);

    if (dir[len - 1] != '/')
        strcat(dir, "/");

    len = strlen(dir);

    for (i = 1; i < len; i++)
    {
        if (dir[i] == '/')
        {
            dir[i] = 0;
            if (access(dir, 0) == -1)
            {
                if (mkdir(dir) == -1)
                {
                    dprif("mkdir %s error\n", dirpath);
                    return -2;
                }
            }
            else
            {
                //dprif("%s exist\n", dir);
            }
            dir[i] = '/';
        }
    }

    return 0;
}

int PictureToMallocDotArray(const char *bmppath, uint16 **dotbuf, uint32 *size, Coord_t *coord, const PictureKarma_t *karma)
{
    FILE *fp;
    BMPFileHeader_t bmpfileheader;
    BMPInfoHeader_t bmpinfoheader;
    int16 bmpdatawidth;
    int32 i,j,k;
    RGB32_t RGB32;
    RGB24_t RGB24;
    uint16 rgbdata[3];
    uint16 rgbdatatmp;
    uint16 *dotarray;

    *size = 0;

    if ((bmppath == NULL) || (dotbuf == NULL) || (size == NULL))
    {
        dprif("ConvertPictureToDotArray %s parameter error\n", bmppath);
        return GETPICTURE_ERROR;
    }

    dprif("fopen BMPpath %s\n", bmppath);
    if((fp = fopen(bmppath, "rb")) == NULL)
    {
        dprif("ConvertPictureToDotArray %s path error\n", bmppath);
        return GETPICTURE_NOFILE;
    }

    fread(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
    fread(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

    if((0 != memcmp((char*)&bmpfileheader.Type, "BM", 2))
     &&(0 != memcmp((char*)&bmpfileheader.Type, "MB", 2)))
    {
        fclose(fp);
        dprif("ConvertPictureToDotArray %s not bmp file\n", bmppath);
        return GETPICTURE_NOTBMP;
    }

    bmpdatawidth = (bmpinfoheader.Width + 15)/16;

    if (coord != NULL)
    {
        if (GET_GRAY_LEVEL(karma->BaseColor) == 2)
        {
            coord->X = 0;
            coord->Y = 0;
            coord->Width = bmpinfoheader.Width;
            coord->Height = bmpinfoheader.Height;
        }
        else // NO GRAY
        {
            coord->X = bmpdatawidth;
            coord->Y = bmpdatawidth;
            coord->Width = bmpdatawidth*16;
            coord->Height = bmpinfoheader.Height;
        }
    }

    if (size != NULL)
    {
        if (GET_GRAY_LEVEL(karma->BaseColor) == 2)
        {
            (*size) = bmpinfoheader.Width * bmpinfoheader.Height;
        }
        else // NO GRAY
        {
            (*size) = bmpdatawidth * bmpinfoheader.Height * GET_BASE_COLOR(karma->BaseColor) * sizeof(uint16);
        }
    }

    *dotbuf = malloc(*size);
    if (*dotbuf == NULL)
    {
        fclose(fp);
        dprif("ConvertPictureToDotArray malloc error\n");
        return GETPICTURE_ERROR;
    }
    dotarray = *dotbuf;

    dprif("W:%ld H:%ld, Size:%lu\n", bmpinfoheader.Width, bmpinfoheader.Height, *size);

    if(bmpinfoheader.BitCount == 24)
    {
        if (GET_GRAY_LEVEL(karma->BaseColor) == 2)
        {
            uint8 *dotarray8b = *dotbuf;
            for(i = 0; i < bmpinfoheader.Height; ++i)
            {
                for(j = 0; j < bmpinfoheader.Width; ++j)
                {
                    int pos = (bmpinfoheader.Height - 1 - i)*bmpinfoheader.Width + j;
                    fread(&RGB24, sizeof(RGB24_t), 1, fp);
                    dotarray8b[pos] = 0;
                    if (karma->Gama[RGB24.Red] & 0x02) dotarray8b[pos] |= 1 << 0;
                    if (karma->Gama[RGB24.Green] & 0x02) dotarray8b[pos] |= 1 << 1;
                    if (karma->Gama[RGB24.Blue] & 0x02) dotarray8b[pos] |= 1 << 2;
                    if (karma->Gama[RGB24.Red] & 0x01) dotarray8b[pos] |= 1 << 4;
                    if (karma->Gama[RGB24.Green] & 0x01) dotarray8b[pos] |= 1 << 5;
                    if (karma->Gama[RGB24.Blue] & 0x01) dotarray8b[pos] |= 1 << 6;
                    if (!karma->DataPolar) dotarray8b[pos] = ~dotarray8b[pos];
                }

                if(0 != (bmpinfoheader.Width*sizeof(RGB24_t)%4))
                {
                    fread(&RGB32, 4 - (bmpinfoheader.Width*sizeof(RGB24_t)%4), 1, fp);
                }
            }
        }
        else
        {
            for(i = 0; i < bmpinfoheader.Height; ++i)
            {
                for(j = 0; j < bmpinfoheader.Width/16; ++j)
                {
                    for(k = 0; k < 3; ++k)
                    {
                        rgbdata[k] =0;
                    }

                    for(k = 0; k < 16; ++k)
                    {
                        fread(&RGB24, sizeof(RGB24_t), 1, fp);
                        if(RGB24.Red > karma->ColorThreshold)
                        {
                            rgbdata[0] |= (1ul<<(15-k));
                        }
                        if(RGB24.Green > karma->ColorThreshold)
                        {
                            rgbdata[1] |= (1ul<<(15-k));
                        }
                        if(RGB24.Blue > karma->ColorThreshold)
                        {
                            rgbdata[2] |= (1ul<<(15-k));
                        }
                    }
                    if(karma->Endian == FILE_BIG_ENDIAN)
                    {
                        for(k = 0; k < 3; ++k)
                        {
                            rgbdatatmp = rgbdata[k]&0xff;
                            rgbdata[k] >>= 8;
                            rgbdata[k] |= (rgbdatatmp <<8);
                        }
                    }

                    for(k = 0; k < GET_BASE_COLOR(karma->BaseColor); ++k)
                    {
                        dotarray[bmpdatawidth*bmpinfoheader.Height*k
                            +bmpdatawidth*(bmpinfoheader.Height -1 -i) + j] = rgbdata[k];
                    }
                }

                if(0 != (bmpinfoheader.Width%16))
                {
                    for(k = 0; k < 3; ++k)
                    {
                        rgbdata[k] =0;
                    }

                    for(k = 0; k < bmpinfoheader.Width%16; ++k)
                    {
                        fread(&RGB24, sizeof(RGB24_t), 1, fp);
                        if(RGB24.Red > karma->ColorThreshold)
                        {
                            rgbdata[0] |= (1ul<<(15-k));
                        }

                        if(RGB24.Green > karma->ColorThreshold)
                        {
                            rgbdata[1] |= (1ul<<(15-k));
                        }

                        if(RGB24.Blue > karma->ColorThreshold)
                        {
                            rgbdata[2] |= (1ul<<(15-k));
                        }
                    }

                    if(karma->Endian == FILE_BIG_ENDIAN)
                    {
                        for(k = 0; k < 3; ++k)
                        {
                            rgbdatatmp = rgbdata[k]&0xff;
                            rgbdata[k] >>= 8;
                            rgbdata[k] |= (rgbdatatmp <<8);
                        }
                    }

                    for(k = 0; k < GET_BASE_COLOR(karma->BaseColor); ++k)
                    {
                        dotarray[bmpdatawidth*bmpinfoheader.Height*k
                            +bmpdatawidth*(bmpinfoheader.Height -1 -i) + j] =rgbdata[k];
                    }
                }

                if(0 != (bmpinfoheader.Width*sizeof(RGB24_t)%4))
                {
                    fread(&RGB32, 4 - (bmpinfoheader.Width*sizeof(RGB24_t)%4), 1, fp);
                }
            }
        }
    }
    else if(bmpinfoheader.BitCount == 32)
    {
        if (GET_GRAY_LEVEL(karma->BaseColor) == 2)
        {
            uint8 *dotarray8b = *dotbuf;
            for(i = 0; i < bmpinfoheader.Height; ++i)
            {
                for(j = 0; j < bmpinfoheader.Width; ++j)
                {
                    int pos = (bmpinfoheader.Height - 1 - i)*bmpinfoheader.Width + j;
                    fread(&RGB32, sizeof(RGB32_t), 1, fp);
                    dotarray8b[pos] = 0;
                    if (karma->Gama[RGB24.Red] & 0x02) dotarray8b[pos] |= 1 << 0;
                    if (karma->Gama[RGB24.Green] & 0x02) dotarray8b[pos] |= 1 << 1;
                    if (karma->Gama[RGB24.Blue] & 0x02) dotarray8b[pos] |= 1 << 2;
                    if (karma->Gama[RGB24.Red] & 0x01) dotarray8b[pos] |= 1 << 4;
                    if (karma->Gama[RGB24.Green] & 0x01) dotarray8b[pos] |= 1 << 5;
                    if (karma->Gama[RGB24.Blue] & 0x01) dotarray8b[pos] |= 1 << 6;
                    if (!karma->DataPolar) dotarray8b[pos] = ~dotarray8b[pos];
                }
            }
        }
        else
        {
            for(i = 0; i < bmpinfoheader.Height; ++i)
            {
                for(j = 0; j < bmpinfoheader.Width/16; ++j)
                {
                    for(k = 0; k < 3; ++k)
                    {
                        rgbdata[k] = 0;
                    }

                    for(k = 0; k < 16; ++k)
                    {
                        fread(&RGB32, sizeof(RGB32_t), 1, fp);
                        if(RGB32.Red > karma->ColorThreshold)
                        {
                            rgbdata[0] |= (1ul<<(15-k));
                        }
                        if(RGB32.Green > karma->ColorThreshold)
                        {
                            rgbdata[1] |= (1ul<<(15-k));
                        }
                        if(RGB32.Blue > karma->ColorThreshold)
                        {
                            rgbdata[2] |= (1ul<<(15-k));
                        }
                    }

                    if(karma->Endian == FILE_BIG_ENDIAN)
                    {
                        for(k = 0; k < 3; ++k)
                        {
                            rgbdatatmp = rgbdata[k]&0xff;
                            rgbdata[k] >>= 8;
                            rgbdata[k] |= (rgbdatatmp << 8);
                        }
                    }

                    for(k = 0; k < GET_BASE_COLOR(karma->BaseColor); ++k)
                    {
                        dotarray[bmpdatawidth*bmpinfoheader.Height*k
                            +bmpdatawidth*(bmpinfoheader.Height -1 -i) + j] = rgbdata[k];
                    }
                }

                if(0 != (bmpinfoheader.Width%16))
                {
                    for(k = 0; k < 3; ++k)
                    {
                        rgbdata[k] = 0;
                    }

                    for(k = 0; k < bmpinfoheader.Width%16; ++k)
                    {
                        fread(&RGB32, sizeof(RGB32_t), 1, fp);
                        if(RGB32.Red > karma->ColorThreshold)
                        {
                            rgbdata[0] |= (1ul<<(15-k));
                        }

                        if(RGB32.Green > karma->ColorThreshold)
                        {
                            rgbdata[1] |= (1ul<<(15-k));
                        }

                        if(RGB32.Blue > karma->ColorThreshold)
                        {
                            rgbdata[2] |= (1ul<<(15-k));
                        }
                    }

                    if(karma->Endian == FILE_BIG_ENDIAN)
                    {
                        for(k = 0; k < 3; ++k)
                        {
                            rgbdatatmp = rgbdata[k]&0xff;
                            rgbdata[k] >>= 8;
                            rgbdata[k] |= (rgbdatatmp <<8);
                        }
                    }

                    for(k = 0; k < GET_BASE_COLOR(karma->BaseColor); ++k)
                    {
                        dotarray[bmpdatawidth*bmpinfoheader.Height*k
                            +bmpdatawidth*(bmpinfoheader.Height -1 -i) + j] = rgbdata[k];
                    }
                }
            }
        }
    }
    else
    {
        fclose(fp);
        return GETPICTURE_NOT24OR32BITS;
    }

    fclose(fp);

    return GETPICTURE_SUCCESS;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:rgba 32:0 ---- A(8), R(8), G(8), B(8)
 *******************************************************************************
 */
int CreateBMPfile(const char *pathname, uint16 width, uint16 height, uint8 bitdeep, uint32 rgba)
{
    FILE *fp;
    BMPFileHeader_t bmpfileheader;
    BMPInfoHeader_t bmpinfoheader;
    uint32 linebytes;
    uint32 lineremainbytes;
    uint32 bmpposition = sizeof(BMPInfoHeader_t) + sizeof(BMPFileHeader_t);
    int i, j;

    bmpfileheader.Type[0] = 'B';
    bmpfileheader.Type[1] = 'M';
    //bmpfileheader.Size = ?;
    bmpfileheader.Reserved1 = 0;
    bmpfileheader.Reserved2 = 0;
    bmpfileheader.OffBits = sizeof(BMPFileHeader_t) + sizeof(BMPInfoHeader_t);

    bmpinfoheader.Size = sizeof(BMPInfoHeader_t);
    bmpinfoheader.Width = width;
    bmpinfoheader.Height = height;
    bmpinfoheader.Planes = 1;
    if (bitdeep == 24)
        bmpinfoheader.BitCount = 24;
    else if (bitdeep == 32)
        bmpinfoheader.BitCount = 32;
    else
        bmpinfoheader.BitCount = 32;
    bmpinfoheader.Compression = 0;
    bmpinfoheader.SizeImage = 0;
    bmpinfoheader.XPelsPerMeter = 0;
    bmpinfoheader.YPelsPerMeter = 0;
    bmpinfoheader.ClrUsed = 0;
    bmpinfoheader.ClrImportant = 0;

    if (bmpinfoheader.BitCount == 32)
    {
        RGB32_t color;
        color.Blue = (rgba >> 0) & 0xff;
        color.Green = (rgba >> 8) & 0xff;
        color.Red = (rgba >> 16) & 0xff;
        color.Alpha = (rgba >> 24) & 0xff;

        lineremainbytes = 0;
        linebytes = sizeof(RGB32_t) * width + lineremainbytes;
        bmpfileheader.Size = linebytes * height;
        if((fp = fopen(pathname, "wb+")) == NULL)
        {
            dprif("CreateBMPfile pathname %s error\n", pathname);
            return GETPICTURE_NOFILE;
        }

        fseek(fp, 0, SEEK_SET);
        fwrite(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
        fwrite(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

        fseek(fp, bmpposition, SEEK_SET);
        for (i = 0; i < width*height; i++)
            fwrite(&color, sizeof(RGB32_t), 1, fp);
    }
    else if (bmpinfoheader.BitCount == 24)
    {
        RGB24_t color;
        color.Blue = (rgba >> 0) & 0xff;
        color.Green = (rgba >> 8) & 0xff;
        color.Red = (rgba >> 16) & 0xff;

        lineremainbytes = (3*width)%4 ? 4 - (3*width)%4 : 0;
        linebytes = sizeof(RGB24_t) * width + lineremainbytes;
        bmpfileheader.Size = linebytes * height;
        if((fp = fopen(pathname, "wb+")) == NULL)
        {
            dprif("CreateBMPfile pathname %s error\n", pathname);
            return GETPICTURE_NOFILE;
        }

        fseek(fp, 0, SEEK_SET);
        fwrite(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
        fwrite(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

        fseek(fp, bmpposition, SEEK_SET);
        for (j = 0; j < height; j++)
        {
            for (i = 0; i < width; i++)
                fwrite(&color, sizeof(RGB24_t), 1, fp);
            fwrite(&color, lineremainbytes, 1, fp);
        }
    }
    else
    {
        dprif("CreateBMPfile bitdeep error\n");
    }

    fclose(fp);
    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CheckAreaCoord(const Coord_t *workarea, Coord_t *area, int16 *startx, int16 *starty)
{
    *startx = 0;
    *starty = 0;

    if ((area->Width < 0) || (area->Height < 0))
    {
        area->X = 0;
        area->Y = 0;
        area->Width = 0;
        area->Height = 0;
        return -1;
    }

    //compare X
    if (area->X < workarea->X)
    {
        if (area->X + area->Width > workarea->X)
        {
            *startx = workarea->X - area->X;
            area->Width = area->Width - *startx;
            area->X = workarea->X;
            if (workarea->X + area->Width > workarea->X + workarea->Width)
            {
                area->Width = workarea->Width;
            }
        }
        else
        {
            area->Width = 0;
        }
    }
    else if (area->X > workarea->X + workarea->Width)
    {
        area->Width = 0;
    }
    else if (area->X + area->Width > workarea->X + workarea->Width)
    {
        area->Width = workarea->X + workarea->Width - area->X;
    }

    //compare Y
    if (area->Y < workarea->Y)
    {
        if (area->Y + area->Height > workarea->Y)
        {
            *starty = workarea->Y - area->Y;
            area->Height = area->Height - *starty;
            area->Y = workarea->Y;
            if (workarea->Y + area->Height > workarea->Y + workarea->Height)
            {
                area->Height = workarea->Height;
            }
        }
        else
        {
            area->Height = 0;
        }
    }
    else if (area->Y > workarea->Y + workarea->Height)
    {
        area->Height = 0;
    }
    else if (area->Y + area->Height > workarea->Y + workarea->Height)
    {
        area->Height = workarea->Y + workarea->Height - area->Y;
    }

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description: srcbit must be equal to dstbit
 *******************************************************************************
 */
int PasteMaterialBMP(const char *dstbmp, const char *srcbmp,
                      int16 dstx, int16 dsty,
                      int16 srcx, int16 srcy,
                      uint8 srcwidth, uint8 srcheight)
{
    FILE *dstfp;
    FILE *srcfp;
    BMPFileHeader_t dstbmpfileheader;
    BMPInfoHeader_t dstbmpinfoheader;
    BMPFileHeader_t srcbmpfileheader;
    BMPInfoHeader_t srcbmpinfoheader;
    uint32 dstlinebytes;
    uint32 dstlineremainbytes;
    uint32 srclinebytes;
    uint32 srclineremainbytes;
    uint32 bmpposition = sizeof(BMPInfoHeader_t) + sizeof(BMPFileHeader_t);
    int i, j;

    if((dstfp = fopen(dstbmp, "rb+")) == NULL)
    {
        dprif("PasteMaterialBMP dstbmp %s error\n", dstbmp);
        return GETPICTURE_NOFILE;
    }
    fseek(dstfp, 0, SEEK_SET);
    fread(&dstbmpfileheader, sizeof(BMPFileHeader_t), 1, dstfp);
    fread(&dstbmpinfoheader, sizeof(BMPInfoHeader_t), 1, dstfp);

    if((srcfp = fopen(srcbmp, "rb")) == NULL)
    {
        dprif("PasteMaterialBMP srcbmp %s error\n", srcbmp);
        fclose(dstfp);
        return GETPICTURE_NOFILE;
    }
    fseek(srcfp, 0, SEEK_SET);
    fread(&srcbmpfileheader, sizeof(BMPFileHeader_t), 1, srcfp);
    fread(&srcbmpinfoheader, sizeof(BMPInfoHeader_t), 1, srcfp);

    dprif("%d, %d\n", dstbmpinfoheader.BitCount, srcbmpinfoheader.BitCount);
    if ((dstbmpinfoheader.BitCount == 32) && (srcbmpinfoheader.BitCount == 32))
    {
        Coord_t srcrange, dstrange;
        int16 srcstartx, srcstarty;

        srclineremainbytes = 0;
        srclinebytes = sizeof(RGB32_t) * srcbmpinfoheader.Width + srclineremainbytes;
        dstlineremainbytes = 0;
        dstlinebytes = sizeof(RGB32_t) * dstbmpinfoheader.Width + dstlineremainbytes;

        dstrange.X = 0;
        dstrange.Y = 0;
        dstrange.Width = dstbmpinfoheader.Width;
        dstrange.Height = dstbmpinfoheader.Height;
        srcrange.X = dstx;
        srcrange.Y = dsty;
        srcrange.Width = srcwidth;
        srcrange.Height = srcheight;
        CheckAreaCoord(&dstrange, &srcrange, &srcstartx, &srcstarty);

        char *srclinebuf = malloc(sizeof(RGB32_t)*srcrange.Width);
        for (j = 0; j < srcrange.Height; j++)
        {
            fseek(srcfp,
                bmpposition +
                srclinebytes * (srcbmpinfoheader.Height - 1 - (srcy + j + srcstarty)) +
                (srcx + srcstartx) * sizeof(RGB32_t),
                SEEK_SET);
            fread(srclinebuf, sizeof(RGB32_t), srcrange.Width, srcfp);
            fseek(dstfp,
                bmpposition +
                dstlinebytes * (dstbmpinfoheader.Height - 1 - (j + srcrange.Y)) +
                srcrange.X * sizeof(RGB32_t),
                SEEK_SET);
            fwrite(srclinebuf, sizeof(RGB32_t), srcrange.Width, dstfp);
        }
        free(srclinebuf);
    }
    else if ((dstbmpinfoheader.BitCount == 24) && (srcbmpinfoheader.BitCount == 24))
    {
        Coord_t srcrange, dstrange;
        int16 srcstartx, srcstarty;

        srclineremainbytes = (3*srcbmpinfoheader.Width)%4 ? 4 - (3*srcbmpinfoheader.Width)%4 : 0;
        srclinebytes = sizeof(RGB24_t) * srcbmpinfoheader.Width + srclineremainbytes;
        dstlineremainbytes = (3*dstbmpinfoheader.Width)%4 ? 4 - (3*dstbmpinfoheader.Width)%4 : 0;
        dstlinebytes = sizeof(RGB24_t) * dstbmpinfoheader.Width + dstlineremainbytes;

        dstrange.X = 0;
        dstrange.Y = 0;
        dstrange.Width = dstbmpinfoheader.Width;
        dstrange.Height = dstbmpinfoheader.Height;
        srcrange.X = dstx;
        srcrange.Y = dsty;
        srcrange.Width = srcwidth;
        srcrange.Height = srcheight;
        CheckAreaCoord(&dstrange, &srcrange, &srcstartx, &srcstarty);

        char *srclinebuf = malloc(sizeof(RGB24_t)*srcrange.Width);
        for (j = 0; j < srcrange.Height; j++)
        {
            fseek(srcfp,
                bmpposition +
                srclinebytes * (srcbmpinfoheader.Height - 1 - (srcy + j + srcstarty)) +
                (srcx + srcstartx) * sizeof(RGB24_t),
                SEEK_SET);
            fread(srclinebuf, sizeof(RGB24_t), srcrange.Width, srcfp);
            fseek(dstfp,
                bmpposition +
                dstlinebytes * (dstbmpinfoheader.Height - 1 - (j + srcrange.Y)) +
                srcrange.X * sizeof(RGB24_t),
                SEEK_SET);
            fwrite(srclinebuf, sizeof(RGB24_t), srcrange.Width, dstfp);
        }
        free(srclinebuf);
    }
    else
    {
        dprif("PasteMaterialBMP bitdeep error\n");
    }

    fclose(dstfp);
    fclose(srcfp);
    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#if 0
    #define DOT_DPRINTF(fmt, ...) do { printf(fmt, __VA_ARGS__);fflush(stdout);} while (0)
#else
    #define DOT_DPRINTF(fmt, ...)
#endif

int PrintDotArray(const uint16 *dotarray, const Coord_t *coord, const PictureKarma_t *karma)
{
    /*
    int i, j;
    int loop;
    uint16 *pdotarray;
    uint16 datawidth;

    datawidth = (coord->Width + 15)/16;

    for (loop = 0; loop < GET_BASE_COLOR(karma->BaseColor); loop++)
    {
        pdotarray = (uint16 *)&dotarray[loop*datawidth*coord->Height];

        for (i = 0; i < coord->Width+2; i++)
            DOT_DPRINTF("-");
        DOT_DPRINTF("\n");

        if (karma->Endian == FILE_BIG_ENDIAN)
        {
            uint16 data;
            uint16 tmp;
            for (j = 0; j < coord->Height; j++)
            {
                DOT_DPRINTF("|");
                for (i = 0; i < coord->Width; i++)
                {
                    data = pdotarray[j*datawidth + i/16];
                    tmp = pdotarray[j*datawidth + i/16]&0xff;
                    data >>= 8;
                    data |= (tmp <<8);

                    if (data & (0x8000 >> (i%16)))
                        DOT_DPRINTF("*");
                    else
                        DOT_DPRINTF(" ");
                }
                DOT_DPRINTF("|\n");
            }
        }
        else if (karma->Endian == FILE_LITTLE_ENDIAN)
        {
            for (j = 0; j < coord->Height; j++)
            {
                DOT_DPRINTF("|");
                for (i = 0; i < coord->Width; i++)
                {
                    if (pdotarray[j*datawidth + i/16] & (0x8000 >> (i%16)))
                        DOT_DPRINTF("*");
                    else
                        DOT_DPRINTF(" ");
                }
                DOT_DPRINTF("|\n");
            }
        }
        else
        {
            dprif("PrintDotArray endian error\n");
            return -1;
        }

        for (i = 0; i < coord->Width+2; i++)
            DOT_DPRINTF("-");
        DOT_DPRINTF("\n");
    }
    */

    return 0;
}

int SetRGB(uint8 *red, uint8 *green, uint8 *blue, int color)
{
    switch(color)
    {
        case 1:
            *red = 0x0;
            *green = 0x0;
            *blue = 0x0;
            break;
        case 2:
            *green = 0x0;
            *blue = 0x0;
            break;
        case 3:
            *red = 0x0;
            *blue = 0x0;
            break;
        case 4:
            *red = 0x0;
            *green = 0x0;
            break;
        case 5:
            *blue = 0x0;
            break;
        case 6:
            *green = 0x0;
            break;
        case 7:
            *red = 0x0;
            break;
        default:
            break;
    }
}

/*
 *******************************************************************************
 * author:
 * date:
 * description: color = 0bmp90
 * 1-8
 *******************************************************************************
 */
int CreatSpecialBMP(const char *bmppath, const char *dir, const char *name, int number, int color)
{
    FILE *fp;
    FILE *cfp;
    char path[PATH_LEN];
    int32 i,j;
    BMPFileHeader_t bmpfileheader;
    BMPInfoHeader_t bmpinfoheader;
    uint32 bmpposition = sizeof(BMPInfoHeader_t) + sizeof(BMPFileHeader_t);

    RGB32_t RGB32;
    RGB24_t RGB24;

    if ((fp = fopen(bmppath, "rb")) == NULL)
    {
        return GETPICTURE_NOFILE;
    }

    fread(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
    fread(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

    if((0 !=memcmp((char*)&bmpfileheader.Type, "BM", 2))
      &&(0 !=memcmp((char*)&bmpfileheader.Type, "MB", 2)))
    {
        fclose(fp);
        return GETPICTURE_NOTBMP;
    }

    //
    CreateDirectoryK(dir);
    if (dir[strlen(dir) - 1] != '/')
        sprintf(path, "%s/%s_%d.bmp", dir, name, number);
    else
        sprintf(path, "%s%s_%d.bmp", dir, name, number);

    if(bmpinfoheader.BitCount == 24)
    {
        uint32 offset = 0;

        if(color != 0)
        {
            CreateBMPfile(path, bmpinfoheader.Width, bmpinfoheader.Height, 24, 0);
        }
        else //bmp
        {
            CreateBMPfile(path, bmpinfoheader.Height, bmpinfoheader.Width, 24, 0);
        }
        if ((cfp = fopen(path, "rb+")) == NULL)
        {
            fclose(fp);
            return GETPICTURE_NOFILE;
        }

        //4
        uint32 srcremain = (3*bmpinfoheader.Width)%4 ? 4 - (3*bmpinfoheader.Width)%4 : 0;
        uint32 dstremain = (3*bmpinfoheader.Height)%4 ? 4 - (3*bmpinfoheader.Height)%4 : 0;
        for(i = 0; i < bmpinfoheader.Height; ++i)
        {
            for(j = 0; j < bmpinfoheader.Width; ++j)
            {
                fread(&RGB24, sizeof(RGB24_t), 1, fp);
                if(color != 0)
                {
                    if(RGB24.Red > 0)
                    {
                        SetRGB(&RGB24.Red, &RGB24.Green, &RGB24.Blue ,color);
                    }
                    offset = i * (bmpinfoheader.Width * sizeof(RGB24_t) + srcremain) + j * sizeof(RGB24_t);
                }
                else
                {
                    offset = (bmpinfoheader.Width - j - 1) * (bmpinfoheader.Height * sizeof(RGB24_t) + dstremain) + i * sizeof(RGB24_t);
                }
                fseek(cfp,bmpposition+offset,SEEK_SET);
                fwrite(&RGB24, sizeof(RGB24_t), 1, cfp);
            }
            fseek(fp,srcremain,SEEK_CUR);
        }
        fclose(cfp);
    }
    else if(bmpinfoheader.BitCount == 32)
    {
        uint32 offset = 0;

        if(color != 0)
        {
            CreateBMPfile(path, bmpinfoheader.Width, bmpinfoheader.Height, 32, 0);
        }
        else //bmp
        {
            CreateBMPfile(path, bmpinfoheader.Height, bmpinfoheader.Width, 32, 0);
        }
        if ((cfp = fopen(path, "rwb+")) == NULL)
        {
            fclose(fp);
            return GETPICTURE_NOFILE;
        }

        for(i = 0; i < bmpinfoheader.Height; ++i)
        {
            for(j = 0; j < bmpinfoheader.Width; ++j)
            {
                fread(&RGB32, sizeof(RGB32_t), 1, fp);
                if(color != 0)
                {
                    if(RGB32.Red > 0)
                    {
                        SetRGB(&RGB32.Red, &RGB32.Green, &RGB32.Blue ,color);
                    }
                    offset = i * (bmpinfoheader.Width * sizeof(RGB32_t)) + j * sizeof(RGB32_t);
                }
                else
                {
                    offset = (bmpinfoheader.Width - j - 1) * bmpinfoheader.Height * sizeof(RGB32_t) + i * sizeof(RGB32_t);
                }
                fseek(cfp,bmpposition+offset,SEEK_SET);
                fwrite(&RGB32, sizeof(RGB32_t), 1, cfp);
            }
        }
        fclose(cfp);
    }
    else
    {
        dprif("BMP bitdeep error\n");
    }

    fclose(fp);
    return GETPICTURE_SUCCESS;
}

#if 0
//bmp90
int RotateBMPFile(const char *bmppath, const char *dir, const char *name, int number)
{
    FILE *fp;
    FILE *cfp;
    char path[PATH_LEN];
    int32 i,j;
    BMPFileHeader_t bmpfileheader;
    BMPInfoHeader_t bmpinfoheader;
    uint32 bmpposition = sizeof(BMPInfoHeader_t) + sizeof(BMPFileHeader_t);

    RGB32_t RGB32;
    RGB24_t RGB24;

    if ((fp = fopen(bmppath, "rb")) == NULL)
    {
        return GETPICTURE_NOFILE;
    }

    fread(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
    fread(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

    if((0 !=memcmp((char*)&bmpfileheader.Type, "BM", 2))
      &&(0 !=memcmp((char*)&bmpfileheader.Type, "MB", 2)))
    {
        fclose(fp);
        return GETPICTURE_NOTBMP;
    }

    CreateDirectory(dir);
    if (dir[strlen(dir) - 1] != '/')
        sprintf(path, "%s/%s_%d.bmp", dir, name, number);
    else
        sprintf(path, "%s%s_%d.bmp", dir, name, number);

    if(bmpinfoheader.BitCount == 24)
    {
        //bmp
        CreateBMPfile(path, bmpinfoheader.Height, bmpinfoheader.Width, 24, 0);
        if ((cfp = fopen(path, "rwb+")) == NULL)
        {
            fclose(fp);
            return GETPICTURE_NOFILE;
        }

        //4
        uint32 srcremain = (3*bmpinfoheader.Width)%4 ? 4 - (3*bmpinfoheader.Width)%4 : 0;
        uint32 dstremain = (3*bmpinfoheader.Height)%4 ? 4 - (3*bmpinfoheader.Height)%4 : 0;
        dprif("srcremain %d,dstremain %d\n",srcremain,dstremain);
        for(i = 0; i < bmpinfoheader.Height; ++i)
        {
            for(j = 0; j < bmpinfoheader.Width; ++j)
            {
                fread(&RGB24, sizeof(RGB24_t), 1, fp);
                uint32 offset = (bmpinfoheader.Width - j - 1) * (bmpinfoheader.Height * sizeof(RGB24_t) + dstremain) + i * sizeof(RGB24_t);
                fseek(cfp,bmpposition+offset,SEEK_SET);
                fwrite(&RGB24, sizeof(RGB24_t), 1, cfp);
            }
            fseek(fp,srcremain,SEEK_CUR);
        }
        fclose(cfp);
    }
    else if(bmpinfoheader.BitCount == 32)
    {
        //bmp
        CreateBMPfile(path, bmpinfoheader.Height, bmpinfoheader.Width, 32, 0);
        if ((cfp = fopen(path, "rwb+")) == NULL)
        {
            fclose(fp);
            return GETPICTURE_NOFILE;
        }

        for(i = 0; i < bmpinfoheader.Height; ++i)
        {
            for(j = 0; j < bmpinfoheader.Width; ++j)
            {
                fread(&RGB32, sizeof(RGB32_t), 1, fp);
                uint32 offset = j*bmpinfoheader.Height*sizeof(RGB32_t) + (bmpinfoheader.Height - i - 1)*sizeof(RGB32_t) ;
                fseek(cfp,bmpposition+offset,SEEK_SET);
                fwrite(&RGB32, sizeof(RGB32_t), 1, cfp);
            }
        }
        fclose(cfp);
    }
    else
    {
        dprif("RotateBMP bitdeep error\n");
    }

    fclose(fp);
    return GETPICTURE_SUCCESS;
}
#endif


