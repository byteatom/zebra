#ifndef __PARSE_PROGRAM_H__
#define __PARSE_PROGRAM_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

#include "parse_display.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _TimeBE_
{
    DateTime_t Begintime;
    DateTime_t Endtime;
} TimeBE_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union _TimeUnion_
{
    TimeBE_t TimeBE;
    int32 Timelong;
} TimeUnion_t;
#pragma pack(pop)

#define PROGRAM_DELAY_SEQUENCE              (0)
#define PROGRAM_DELAY_FIXLENGTH             (0)
#define PROGRAM_DELAY_FIXDATE               (1)
#define PROGRAM_DELAY_EXTERNAL              (2)

#define PROGRAM_COMPARE_DATE                (1ul<<15)
#define PROGRAM_COMPARE_TIME                (1ul<<14)

#pragma pack(push, 1)
typedef struct _PlayControl_
{
    TimeUnion_t TimeUnion;
    uint16 ControlMode;
    uint16 Reserved;
} PlayControl_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _CCFProgram_
{
    PlayControl_t PlayControl;
    char ProgramName[SHORT_NAME_LEN];
    char ShineBackgroundName[SHORT_NAME_LEN];
} CCFProgram_t;
#pragma pack(pop)

#define CCF_ADDED_LENGTH                    (SHORT_NAME_LEN)
#pragma pack(push, 1)
typedef struct _CCF_
{
    uint16 ProgramTotal;
    CCFProgram_t *ProgramPtr;
    // -ADD
    char Name[SHORT_NAME_LEN];
} CCF_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _ShineLocus_
{
    int16 PictureNumber;
    int8 OffsetX;
    int8 OffsetY;
    int8 NextPicture;
    uint8 Reserved;
    int16 RecycleTotal;
} ShineLocus_t;
#pragma pack(pop)

#define SHINE_ELEMENT_ADDED_LENGTH      (sizeof(ShineLocus_t *))
#pragma pack(push, 1)
typedef struct _ShineElement_
{
    uint32 LocusOffset;
    uint16 LocusTotal;
    int16 StartX;
    int16 StartY;
    int16 StartPictureNumber;
    // -ADD
    ShineLocus_t *LocusPtr;
} ShineElement_t;
#pragma pack(pop)

//
#define SHINE_PICTURE_ADDED_LENGTH      (sizeof(char)*PATH_LEN)
#pragma pack(push, 1)
typedef struct _ShinePicture_
{
    uint32 PictureOffset;
    int16 Width;
    int16 Height;
    // -ADD
    char PictureContent[PATH_LEN];
} ShinePicture_t;
#pragma pack(pop)

#define BCF_ADDED_LENGTH                (sizeof(ShineElement_t *) + sizeof(ShinePicture_t *) + SHORT_NAME_LEN)
#pragma pack(push, 1)
typedef struct _BCF_
{
    uint16 Type;
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
    char Name[SHORT_NAME_LEN];
} BCF_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define WATER_EDGING_MODE_ENABLE        (1ul<<7)
#define WE_STATIC                       (1ul)
#define WE_FORWARD                      (2ul)
#define WE_REVERSAL                     (0ul)
#define WATER_EDGING_MODE(m)            (m<<5)
#define WATER_EDGING_FLICKER            (1ul<<0)

#define WATER_EDGING_ADDED_LENGTH       (sizeof(char)*PATH_LEN*4)
#pragma pack(push, 1)
typedef struct _WaterEdging_
{
    uint32 PictureOffset;
    int8 Width;
    int8 Height;
    uint8 Speed;
    uint8 Mode;
    // -ADD
    char PictureUpContent[PATH_LEN];
    char PictureDownContent[PATH_LEN];
    char PictureLeftContent[PATH_LEN];
    char PictureRightContent[PATH_LEN];
} WaterEdging_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define AREA_WORD               (1)
#define AREA_TIME               (2)
#define AREA_WATCH              (3)
#define AREA_COUNT              (4)
#define AREA_LUNAR              (5)
#define AREA_SHINE              (7)
#define AREA_SENSOR             (8)
#define AREA_HUMIDITY           (9)
#define AREA_CHARACTER          (10)
#define AREA_COLORFUL           (11)

#define AREA_ADDED_LENGTH       (sizeof(void*) + SHORT_NAME_LEN)
#pragma pack(push, 1)
typedef struct _Area_
{
    uint16 Type;
    Coord_t Coord;
    uint32 PictureAddr;
    WaterEdging_t WaterEdging;
    // -ADD
    void *Ptr;
    char Name[SHORT_NAME_LEN];
} Area_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define PCF_ADDED_LENGTH       (sizeof(Area_t*) + SHORT_NAME_LEN)
#pragma pack(push, 1)
typedef struct _PCF_
{
    uint16 AreaTotal;
    WaterEdging_t WaterEdging;
    // -ADD
    Area_t *Area;
    char Name[SHORT_NAME_LEN];
} PCF_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#pragma pack(push, 1)
typedef struct _ProgramDescribe_
{
    CCF_t CCF;
    PCF_t *PCF;
    BCF_t *BCF;
} ProgramDescribe_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ProgramDescribe(const json_object *new_obj, ProgramDescribe_t *program, Record_t *record);
int JsonParse_AreaArray(const json_object *new_obj, Area_t **area, uint16 *number, char *id, Record_t *record);
int CreatFile_ProgramCCF(const char *path, CCF_t *ccf, uint8 endian, Record_t *record);
int CreatFile_ProgramBCF(const char *path, BCF_t *bcf, const PictureKarma_t *karma, Record_t *record);
int CreatFile_ProgramPCF(const char *path, PCF_t *pcf, const PictureKarma_t *karma, Record_t *record);
int Inspect_ProgramDescribe(ProgramDescribe_t *program, Record_t *record);

int FwriteWaterEdging(FILE *fp, uint32 offset, const WaterEdging_t *edging, const PictureKarma_t *karma, uint32 *wsize);
int FwritePicture(FILE *fp, uint32 offset, const char *content, const PictureKarma_t *karma, uint32 *wsize);

int JsonParse_ShinePictureArray(const json_object *new_obj, ShinePicture_t **picture, uint16 *number);
int JsonParse_ShineElementArray(const json_object *new_obj, ShineElement_t **element, uint16 *number);
int JsonParse_ShineLocusArray(const json_object *new_obj, ShineLocus_t **locus, uint16 *number);

int CreatFile_ProgramConcord(const char *path, const ProgramDescribe_t *program, uint8 endian, Record_t *record);
int FreeMemory_ProgramCCF(ProgramDescribe_t *program, Record_t *record);
int FreeMemory_ProgramBCF(ProgramDescribe_t *program, Record_t *record);
int FreeMemory_ProgramPCF(ProgramDescribe_t *program, Record_t *record);

#endif // __PARSE_PROGRAM_H__
