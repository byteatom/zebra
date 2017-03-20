#ifndef __PARSE_ACFTIME_H__
#define __PARSE_ACFTIME_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

#include "parse_program.h"

#define TIME_JSON_MODE_1
//#define TIME_JSON_MODE_2

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define ACF_TIME_SHOW_YEARH             (1ul<<15)
#define ACF_TIME_SHOW_YEARL             (1ul<<14)
#define ACF_TIME_SHOW_MONTH             (1ul<<13)
#define ACF_TIME_SHOW_DAY               (1ul<<12)
#define ACF_TIME_SHOW_WEEK              (1ul<<11)
#define ACF_TIME_SHOW_HOUR              (1ul<<10)
#define ACF_TIME_SHOW_MIN               (1ul<<9)
#define ACF_TIME_SHOW_SEC               (1ul<<8)
#define ACF_TIME_SHOW_APM               (1ul<<7)
#define ACF_TIME_24HOURS_MODE           (1ul<<6)

#define ACF_TIME_ADDED_LENGTH           ((1 + 10 + 7 + 13) * PATH_LEN)

#pragma pack(push, 1)
typedef struct _ACFTime_
{
	//Area_t AreaCommon;
	uint16 StartX;
	uint16 StartY;
	uint8 JetLag[4];
	uint16 Reserved;

	uint16 BMask;
	uint8 DateFontWidth;
	uint8 DateFontHeight;
	uint8 DatePictureWidth;
	uint8 DatePictureHeight;
	uint16 DateYearHX;
	uint16 DateYearHY;
	uint16 DateYearLX;
	uint16 DateYearLY;
	uint16 DateMonthX;
	uint16 DateMonthY;
	uint16 DateDayX;
	uint16 DateDayY;
	uint32 DateContentOffset;
	uint8 WeekFontWidth;
	uint8 WeekFontHeight;
	uint8 WeekPictureWidth;
	uint8 WeekPictureHeight;
	uint16 WeekX;
	uint16 WeekY;
	uint32 WeekContentOffset;
	uint8 TimeFontWidth;
	uint8 TimeFontHeight;
	uint8 TimePictureWidth;
	uint8 TimePictureHeight;
	uint16 TimeHourX;
	uint16 TimeHourY;
	uint16 TimeMinX;
	uint16 TimeMinY;
	uint16 TimeSecX;
	uint16 TimeSecY;
	uint32 TimeContentOffset;
	// -ADD
	char BackgroundContent[PATH_LEN];
	char DateContent[10][PATH_LEN];
	char WeekContent[7][PATH_LEN];
	char TimeContent[13][PATH_LEN];
} ACFTime_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFTimeConfig(const json_object *new_obj, ACFTime_t **acf, Record_t *record);
int JsonParse_ACFTimeConfig2(const json_object *new_obj, const Coord_t *coord, const WaterEdging_t *wateredge, ACFTime_t **acf, Record_t *record);
int CreatFile_ACFTime(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFTime(Area_t *area, Record_t *record);

#endif // __PARSE_ACFTIME_H__

