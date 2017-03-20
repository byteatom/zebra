#ifndef __KALER_STRUCT_H__
#define __KALER_STRUCT_H__

#include "kaler_config.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _DateTime_
{
    uint16 Year;
    int8 Month;
    int8 Day;
    uint8 Week;
    int8 Hour;
    int8 Min;
    int8 Sec;
} DateTime_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _HMSTime_
{
    int8 Hour;
    int8 Min;
    int8 Sec;
} HMSTime_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _Coord_
{
    int16 X;
    int16 Y;
    int16 Width;
    int16 Height;
} Coord_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
typedef struct _PictureKarma_
{
    uint8 BaseColor;
    uint8 Endian;
    uint8 ColorThreshold;
    uint8 DataPolar;
    uint8 Reserved;
    uint16 *Gama;
}PictureKarma_t;

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define FILE_VERSION                    (1)
#pragma pack(push, 1)
typedef struct _FileHeader_
{
    char Sign[3];
    uint8 Version;
    uint32 Length;
    uint16 Reserved;
} FileHeader_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define RECORD_FILE_EXIST_NO            (0)
#define RECORD_FILE_EXIST_YES           (0x5a)

#define RECORD_ERRORNO_OK               (0)
#define RECORD_ERRORNO_ERROR            (-1)
#define RECORD_ERRORNO_KEYLACK          (-2)
#define RECORD_ERRORNO_CREATE_FAIL      (-3)
#define RECORD_ERRORNO_CRITICAL         (-4)
typedef struct _Record_
{
    int TopLevelExist;
    int DisplayExist;
    int ScreenExist;
    int ScanDotExist;
    int NetworkExist;
    int ProgramExist;

    int TopLevelErrorno;
    int DisplayErrorno;
    int ScreenErrorno;
    int ScanDotErrorno;
    int NetworkErrorno;
    int ProgramErrorno;
}Record_t;

#endif // __KALER_STRUCT_H__
