#ifndef __PARSE_DISPLAY_H__
#define __PARSE_DISPLAY_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define MAX_BRIGHT                          (16)
#pragma pack(push, 1)
typedef struct _TBright_
{
    uint16 Bright;
    HMSTime_t BeginTime;
    HMSTime_t EndTime;
} TBright_t;
#pragma pack(pop)

#define DISPLAY_TBRIGHT_LENGTH              (4)
#pragma pack(push, 1)
typedef struct _TimingBright_
{
    uint8 TimeNum;
    uint8 TMask;
    uint16 DefaultBright;
    TBright_t TBright[DISPLAY_TBRIGHT_LENGTH];
} TimingBright_t;
#pragma pack(pop)

#define DISPLAY_BMASK_AUTO_BRIGHT           (1<<8)
#define DISPLAY_BMASK_UPDATE_BRIGHT         (1<<7)
#define DISPLAY_BMASK_UPDATE_TIME           (1<<6)
#define DISPLAY_BMASK_AUTO_OPEN_SCREEN      (1<<5)
#define DISPLAY_BMASK_AUTO_CLOSE_SCREEN     (1<<4)
#define DISPLAY_BMASK_RELATIVE_TIME         (1<<3)
//#define DISPLAY_BMASK_POSITIVE              (1<<2)
#define DISPLAY_BMASK_UPDATE_TIMEING_BRIGHT (1<<1)

#define DISPLAY_TIMECMP_ENABLE              (1<<0)
#define DISPLAY_TIMECMP_YEAR                (1<<1)
#define DISPLAY_TIMECMP_MONTH               (1<<2)
#define DISPLAY_TIMECMP_DAY                 (1<<3)
#define DISPLAY_TIMECMP_HOUR                (1<<4)
#define DISPLAY_TIMECMP_MIN                 (1<<5)
#define DISPLAY_TIMECMP_SEC                 (1<<6)
#define DISPLAY_TIMECMP_SUN                 (1<<7)
#define DISPLAY_TIMECMP_MON                 (1<<8)
#define DISPLAY_TIMECMP_TUE                 (1<<9)
#define DISPLAY_TIMECMP_WED                 (1<<10)
#define DISPLAY_TIMECMP_THU                 (1<<11)
#define DISPLAY_TIMECMP_FRI                 (1<<12)
#define DISPLAY_TIMECMP_SAT                 (1<<13)

#pragma pack(push, 1)
typedef struct _DisplayConfig_
{
    uint16 BMask;
    uint16 Bright;
    uint16 CloseScreenMask;
    uint16 OpenScreenMask;
    DateTime_t CloseScreenTime;
    DateTime_t OpenScreenTime;
    DateTime_t CurrentTime;
    TimingBright_t TimingBright;
} DisplayConfig_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_DisplayConfig(const json_object *new_obj, DisplayConfig_t *display, Record_t *record);
#define PARSE_WEEK_MODE_NUMBER    (0)
#define PARSE_WEEK_MODE_MASK      (1)
int JsonParse_DateTime(const json_object *new_obj, DateTime_t *dtime, int mode);
int Inspect_DisplayConfig(DisplayConfig_t *display, Record_t *record);
int CreatFile_DisplayConfig(const char * path, const DisplayConfig_t *display, uint8 endian, Record_t *record);

#endif // __PARSE_DISPLAY_H__
