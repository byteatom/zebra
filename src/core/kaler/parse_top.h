#ifndef __PARSE_TOP_H__
#define __PARSE_TOP_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

#include "parse_display.h"
#include "parse_screen.h"
#include "parse_scandot.h"
#include "parse_network.h"
#include "parse_program.h"

#include "parse_acfword.h"
#include "parse_acftime.h"
#include "parse_acfwatch.h"
#include "parse_acfshine.h"
#include "parse_acfcount.h"
#include "parse_acftemperature.h"
#include "parse_acfcolorful.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
typedef struct _TopLevelConfig_
{
    char OutFilePath[PATH_LEN];
    char LedScreenPath[PATH_LEN];
    char CardModel[NAME_LEN];
    uint8 Endian;
    uint8 ColorThreshold;
}TopLevelConfig_t;

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define JSONOB_TOP_LEVEL_CONFIG     "topLevelConfig"
#define JSONOB_LED_SCREEN_CONFIG    "ledScreenConfig"
#define JSONOB_SCAN_DOT_CONFIG      "scanDotConfig"
#define JSONOB_DISPLAY_CONFIG       "displayConfig"
#define JSONOB_NETWORK_CONFIG       "networkConfig"
#define JSONOB_PROGRAM_DESCRIBE     "programDescribe"
typedef struct _AllConfig_
{
    TopLevelConfig_t TopLevelConfig;
    DisplayConfig_t DisplayConfig;
    ScreenConfig_t ScreenConfig;
    ScanDotConfig_t ScanDotConfig;
    NetworkConfig_t NetworkConfig;
    ProgramDescribe_t ProgramDescribe;
}AllConfig_t;

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParseAndCreateFile(const char *jsonpath);
int JsonParseAll(const char *json_buf, AllConfig_t *allptr, Record_t *record);
int JsonParse_TopLevelConfig(const json_object *new_obj, TopLevelConfig_t *toplevel, Record_t *record);
int Inspect_TopLevelConfig(TopLevelConfig_t *toplevel, Record_t *record);
int CreateKalerFile(const char *outpath, AllConfig_t *allptr, Record_t *record);
int FreeMemoryAll(AllConfig_t *allptr, Record_t *record);

#endif // __PARSE_TOP_H__
