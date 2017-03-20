#ifndef __PARSE_SCREEN_H__
#define __PARSE_SCREEN_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "kaler_macro.h"
#include "../json/json.h"
#include "file_ops.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define SCREEN_BMASK_WARP_MODE              (1ul<<15)
#define SCREEN_BMASK_MIRROR                 (1ul<<14)
#define SCREEN_BMASK_SPLIT                  (1ul<<13)
#define SCREEN_BMASK_DATA_POLAR             (1ul<<12)
#define SCREEN_BMASK_STR_POLAR              (1ul<<11)
#define SCREEN_BMASK_FANCY_WARP             (1ul<<5)
#define SCREEN_BMASK_EXTEND_INFO            (1ul<<4)
#define SCREEN_BMASK_OE_POLAR               (1ul<<3)

#define SCREEN_BASE_COLOR_MASK              (0x07 << 0)
#define SCREEN_GRAY_LEVEL_MASK              (0x1f << 3)
#define GET_BASE_COLOR(color)               (color & SCREEN_BASE_COLOR_MASK)
#define GET_GRAY_LEVEL(color)               ((color & SCREEN_GRAY_LEVEL_MASK) >> 3)

#define SCREEN_CONFIG_ADDED_LENGTH          (PATH_LEN + PATH_LEN + PATH_LEN + NAME_LEN + 256*sizeof(uint16))
#pragma pack(push, 1)
typedef struct  _ScreenConfig_
{
    uint8   Model[16];
    int16   Width;
    int16   Height;
    uint8   ScanLine;
    uint8   ScanMode;
    uint8   WarpBit;
    int8    LineVerify;
    uint8   BaseColor;
    uint8   Frequency;
    uint8   LineBlank;
    uint8   ABCDEMode;
    uint16  BMask;
    uint8   IPAddr[6];
    uint8   SubMask[6];
    uint8   GateWay[6];
    uint16  Port;
    uint8   ServerIPAddr1[6];
    uint16  ServerPort1;
    uint8   ServerIPAddr2[6];
    uint16  ServerPort2;
    uint16  ConfigIndex;
    // -ADD
    char ScreenContent[PATH_LEN];
    char ScanDotContent[PATH_LEN];
    char ScreenPath[PATH_LEN];
    char ScreenName[NAME_LEN];
    uint16 Gama[256];
} ScreenConfig_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ScreenConfig(const json_object *new_obj, ScreenConfig_t *screen, Record_t *record);
int Inspect_ScreenConfig(ScreenConfig_t *screen, Record_t *record);
int CreatFile_ScreenConfig(const char *path, ScreenConfig_t *screen, uint8 endian, Record_t *record);

#endif // __PARSE_SCREEN_H__
