#ifndef __PARSE_ACFWORD_H__
#define __PARSE_ACFWORD_H__

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
#define PICTURE_FUSE_ADDED_LENGTH       (sizeof(char)*PATH_LEN)

#pragma pack(push, 1)
typedef struct _PictureFuse_
{
    uint32 PictureOffset;
    uint8 EnterMode;
    uint8 ExitMode;
    uint8 Speed;
    uint8 Delay;
    uint8 IcoBeginNumber;
    uint8 IcoEndNumber;
    uint8 Cycle;
    uint8 JumpGrid;
    // -ADD
    char PictureContent[PATH_LEN];
} PictureFuse_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _PictureExtend_
{
    int16 Width;
    int16 Height;
    uint16 Reserved1;
    uint16 Reserved2;
} PictureExtend_t;
#pragma pack(pop)

#define ACF_WORD_ADDED_LENGTH           (sizeof(PictureFuse_t*) + sizeof(PictureExtend_t*))
#pragma pack(push, 1)
typedef struct _ACFWord_
{
    //Area_t AreaCommon;
    uint16 PictureTotal;
    uint16 IcoTotal;
    uint32 IcoParaAddr;
    uint32 Reserved;
    // -ADD
    PictureFuse_t *PictureFusePtr;
    PictureExtend_t *PictureExtendPtr;
} ACFWord_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFWordConfig(const json_object *new_obj, ACFWord_t **acf, Record_t *record);
int CreatFile_ACFWord(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFWord(Area_t *area, Record_t *record);

#endif // __PARSE_ACFWORD_H__
