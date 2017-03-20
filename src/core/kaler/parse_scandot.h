#ifndef __PARSE_SCANDOT_H__
#define __PARSE_SCANDOT_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "kaler_macro.h"
#include "../json/json.h"
#include "file_ops.h"
#include "parse_screen.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define SCANDOT_BMASK_ABCDE_MODE        (1ul<<15)
#define SCANDOT_BMASK_OE_POLAR          (1ul<<14)
#define SCANDOT_BMASK_DATA_POLAR        (1ul<<12)
#define SCANDOT_BMASK_STR_POLAR         (1ul<<11)
#define SCANDOT_BMASK_BLANK_SCAN        (1ul<<10)

#pragma pack(push, 1)
typedef struct _DotMap_
{
    uint8 y;
    uint8 x;
} DotMap_t;
#pragma pack(pop)

#define SCANDOT_DOTMAP_LENGTH           (256)
#define SCANDOT_LINEMAP_LENGTH          (16)
#pragma pack(push, 1)
typedef struct _ScanDotConfig_
{
    DotMap_t DotMap[256];
    uint8 LineMap[16];
    uint16 BMask;
    uint16 ConfigIndex;
    uint8 ScanLine;
    uint8 WarpTimes;
    uint8 Gama;
    uint8 Width;
    uint8 EmptyDot;
    uint8 Reserved;
} ScanDotConfig_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ScanDotConfig(const json_object *new_obj, ScanDotConfig_t *scandot, Record_t *record);
int Inspect_ScanDotConfig(ScanDotConfig_t *scandot, Record_t *record);
int CreatFile_ScanDotConfig(const char *path, const ScanDotConfig_t *scandot, const ScreenConfig_t *screen, uint8 endian, Record_t *record);

#endif // __PARSE_SCANDOT_H__
