#ifndef __PARSE_ACFCOUNT_H__
#define __PARSE_ACFCOUNT_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

#include "parse_program.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define ACF_COUNT_SHOW_DAY               (1ul<<15)
#define ACF_COUNT_SHOW_HOUR              (1ul<<14)
#define ACF_COUNT_SHOW_MIN               (1ul<<13)
#define ACF_COUNT_SHOW_SEC               (1ul<<12)
#define ACF_COUNT_EN_TURN                (1ul<<11)

#define ACF_COUNT_ADDED_LENGTH           ((1 + 1 + 10) * PATH_LEN)

#pragma pack(push, 1)
typedef struct _ACFCount_
{
    //Area_t AreaCommon;
    uint8 DayWidth;
    uint8 HourWidth;
    uint8 MinWidth;
    uint8 SecWidth;
    uint16 Reserved;
    
    uint16 BMask;
    DateTime_t AimTime;
    uint8 DigitFontWidth;
    uint8 DigitFontHeight;
    uint8 DigitPictureWidth;
    uint8 DigitPictureHeight;
    uint16 DigitX;
    uint16 DigitY;
    uint32 DigitContentOffset;

    uint16 SecondPictureWidth;
    uint16 SecondPictureHeight;
    uint32 SecondContentOffset;
    // -ADD
    char BackgroundContent[PATH_LEN];
    char SecondContent[PATH_LEN];
    char DigitContent[10][PATH_LEN];
} ACFCount_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFCountConfig(const json_object *new_obj, ACFCount_t **acf, Record_t *record);
int CreatFile_ACFCount(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFCount(Area_t *area, Record_t *record);

#endif // __PARSE_ACFCOUNT_H__

