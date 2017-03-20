#ifndef __PARSE_ACFWATCH_H__
#define __PARSE_ACFWATCH_H__

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

#pragma pack(push, 1)
typedef struct _WatchHand_
{
    int16 X;
    int16 Y;
    int16 Width;
    int16 Height;
    uint32 PictureOffset;
} WatchHand_t;
#pragma pack(pop)

#define ACF_WATCH_ADDED_LENGTH           ((1 + 180) * PATH_LEN)
#pragma pack(push, 1)
typedef struct _ACFWatch_
{
    //Area_t AreaCommon;
    uint32 ParameterOffset;
    uint8 JetLag[4];

    WatchHand_t HourHand[60];
    WatchHand_t MinHand[60];
    WatchHand_t SecHand[60];

    uint16 Reserved;

    // -ADD
    char BackgroundContent[PATH_LEN];
    char HourHandContent[60][PATH_LEN];
    char MinHandContent[60][PATH_LEN];
    char SecHandContent[60][PATH_LEN];
} ACFWatch_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFWatchConfig(const json_object *new_obj, ACFWatch_t **acf, Record_t *record);
int CreatFile_ACFWatch(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFWatch(Area_t *area, Record_t *record);

#endif // __PARSE_ACFWATCH_H__

