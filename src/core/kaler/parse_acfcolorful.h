#ifndef __PARSE_ACFCOLORFUL_H__
#define __PARSE_ACFCOLORFUL_H__

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
#define PICTURE_FUSE2_ADDED_LENGTH       (sizeof(char)*PATH_LEN)

#pragma pack(push, 1)
typedef struct _PictureFuse2_
{
    uint32 PictureOffset;
    int16 StartX;
    int16 StartY;
    int16 Width;
    int16 Height;
    char PictureContent[PATH_LEN];
} PictureFuse2_t;
#pragma pack(pop)

#define ACF_COLORFUL_ADDED_LENGTH           (sizeof(PictureFuse2_t*))
#pragma pack(push, 1)
typedef struct _ACFColorful_
{
    //Area_t AreaCommon;
    uint16 PictureTotal;
    uint16 Speed;
    uint8 Display;
    uint8 Reserved8;
    uint16 Reserved16;
    // -ADD
    PictureFuse2_t *PictureFusePtr;
} ACFColorful_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFColorfulConfig(const json_object *new_obj, ACFColorful_t **acf, Record_t *record);
int CreatFile_ACFColorful(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record);
int FreeMemory_ACFColorful(Area_t *area, Record_t *record);

#endif // __PARSE_ACFCOLORFUL_H__
