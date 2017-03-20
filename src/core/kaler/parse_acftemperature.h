#ifndef __PARSE_ACFTEMPERATURE_H__
#define __PARSE_ACFTEMPERATURE_H__

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
#define ACF_TEMPERATURE_SHOW_FAHRENHEIT     (1ul<<15)
#define ACF_TEMPERATURE_SHOW_HUMIDITY       (1ul<<14)

#define ACF_TEMPERATURE_ADDED_LENGTH        ((1 + 1 + 12) * PATH_LEN)

#pragma pack(push, 1)
typedef struct _ACFTemperature_
{
    //Area_t AreaCommon;
    int16 Adjust;
    uint16 BMask;
    
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
    char DigitContent[12][PATH_LEN];
} ACFTemperature_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFTemperatureConfig(const json_object *new_obj, ACFTemperature_t **acf, Record_t *record);
int CreatFile_ACFTemperature(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFTemperature(Area_t *area, Record_t *record);

#endif // __PARSE_ACFTEMPERATURE_H__

