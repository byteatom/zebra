#ifndef __FILE_OPS_H__
#define __FILE_OPS_H__

#include "kaler_config.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _BMPFileHeader_
{
	uint8 Type[2];
	uint32 Size;
	uint16 Reserved1;
	uint16 Reserved2;
	uint32 OffBits;
} BMPFileHeader_t;

typedef struct _BMPInfoHeader_
{
	uint32 Size;
	int32 Width;
	int32 Height;
	uint16 Planes;
	uint16 BitCount;
	uint32 Compression;
	uint32 SizeImage;
	int32 XPelsPerMeter;
	int32 YPelsPerMeter;
	uint32 ClrUsed;
	uint32 ClrImportant;
} BMPInfoHeader_t;

typedef struct _RGB32_
{
	uint8 Blue;
	uint8 Green;
	uint8 Red;
	uint8 Alpha;
} RGB32_t;

typedef struct _RGB24_
{
	uint8 Blue;
	uint8 Green;
	uint8 Red;
} RGB24_t;
#pragma pack(pop)

#define GETPICTURE_SUCCESS          (0)
#define GETPICTURE_NOFILE           (-1)	//1
#define GETPICTURE_NOTBMP           (-2)
#define GETPICTURE_NOT24OR32BITS    (-3)//3
#define GETPICTURE_ERROR            (-4)//3

int CheckCPU(void);
uint32 GetFileSizeK(FILE* fp);
int DetachFilePath(const char *path, char *dir, char *file);
int InitJsonFileBuf(const char *path, char ** const buf);
void FreeJsonFileBuf(char *buf);
int CreateDirectoryK(const char *dirpath);
int PictureToMallocDotArray(const char *bmppath, uint16 **dotbuf, uint32 *size, Coord_t *coord, const PictureKarma_t *karma);
int PrintDotArray(const uint16 *dotarray, const Coord_t *coord, const PictureKarma_t *karma);
int CreateBMPfile(const char *pathname, uint16 width, uint16 height, uint8 bitdeep, uint32 rgba);
int PasteMaterialBMP(const char *dstbmp, const char *srcbmp,
					  int16 dstx, int16 dsty,
					  int16 srcx, int16 srcy,
					  uint8 srcwidth, uint8 srcheight);
int CreatSpecialBMP(const char *bmppath, const char *dir, const char *name, int number, int color);

#endif // __FILE_OPS_H__
