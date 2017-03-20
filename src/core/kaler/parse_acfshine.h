#ifndef __PARSE_ACFSHINE_H__
#define __PARSE_ACFSHINE_H__

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
#define ACF_SHINE_ADDED_LENGTH          (sizeof(ShineElement_t *) + sizeof(ShinePicture_t *))

#pragma pack(push, 1)
typedef struct _ACFShine_
{
    //Area_t AreaCommon;
    uint16 FrameTotal;
    uint16 ElementTotal;
    uint16 PictureTotal;
    uint8 FrameSpeed;
    uint8 Reserved;
    uint32 ElementAddr;
    uint32 PictureAddr;
    // -ADD
    ShineElement_t *ElementPtr;
    ShinePicture_t *PicturePtr;
} ACFShine_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFShineConfig(const json_object *new_obj, ACFShine_t **acf, Record_t *record);
int CreatFile_ACFShine(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFShine(Area_t *area, Record_t *record);

#endif // __PARSE_ACFSHINE_H__

