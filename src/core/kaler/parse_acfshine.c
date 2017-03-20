#include "kaler_config.h"
#include "../json/json.h"
#include "parse_acfshine.h"

#ifdef undprif
#undef dprif
#define dprif(fmt, ...)
#endif

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFShineConfig(const json_object *new_obj, ACFShine_t **acf, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;
    ACFShine_t *pacf;

    *acf = malloc(sizeof(ACFShine_t));
    memset(*acf, 0, sizeof(ACFShine_t));

    pacf = *acf;

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "frameTotal"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->FrameTotal = vint;
                dprif("FrameTotal : %d\n", pacf->FrameTotal);
            }
            else
            {
                dprif("FrameTotal error,current value %d\n", pacf->FrameTotal);
            }
        }
        else if (!strcmp(key, "pictureTotal"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->PictureTotal = vint;
                dprif("PictureTotal : %d\n", pacf->PictureTotal);
            }
            else
            {
                dprif("PictureTotal error,current value %d\n", pacf->PictureTotal);
            }
        }
        else if (!strcmp(key, "elementTotal"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->ElementTotal = vint;
                dprif("ElementTotal : %d\n", pacf->ElementTotal);
            }
            else
            {
                dprif("ElementTotal error,current value %d\n", pacf->ElementTotal);
            }
        }
        else if (!strcmp(key, "frameSpeed"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->FrameSpeed = vint;
                dprif("FrameSpeed : %d\n", pacf->FrameSpeed);
            }
            else
            {
                dprif("FrameSpeed error,current value %d\n", pacf->FrameSpeed);
            }
        }
        else if (!strcmp(key, "pictureArray"))
        {
            JsonParse_ShinePictureArray(val, &pacf->PicturePtr, &pacf->PictureTotal);
        }
        else if (!strcmp(key, "elementArray"))
        {
            JsonParse_ShineElementArray(val, &pacf->ElementPtr, &pacf->ElementTotal);
        }
        else
        {
            dprif("ACFShineConfig key \"%s\" can not recognized\n", key);
        }
    }

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CreatFile_ACFShine(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record)
{
    int i;
    uint32 cnt;
    uint32 modelcnt;
    FILE *fp = NULL;
    FileHeader_t header;
    uint32 wcnt = 0;

    dprif("CreatFile_ACFShine\n");

    if ((path == NULL) || (area == NULL))
    {
        dprif("CreatFile_ACFShine parameter error\n");
        goto out;
    }

    if (path[strlen(path) - 1] != '/')
    {
        dprif("CreatFile_ACFShine path error\n");
        goto out;
    }

    if (CreateDirectoryK(path) < 0)
        goto out;

    char file[PATH_LEN];

    sprintf(file, "%s%s", path, area->Name);
    memcpy(header.Sign, "ACF", 3);

    if ((fp = fopen(file, "wb+")) == NULL) {
        dprif("fopen %s failed\n", file);
        goto out;
    }

    //-----------------------------------------------------------------------//
    ACFShine_t *acf = area->Ptr;
    uint32 specific_para_addr;
    //uint32 picture_para_addr;

    //-----------------------------------------------------------------------//
    wcnt += sizeof(FileHeader_t) + sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
    wcnt -= 4;
    specific_para_addr = wcnt;

    wcnt += sizeof(ACFShine_t) - ACF_SHINE_ADDED_LENGTH;
    acf->ElementAddr = wcnt;
    wcnt += acf->ElementTotal * (sizeof(ShineElement_t) - SHINE_ELEMENT_ADDED_LENGTH);

    acf->PictureAddr = wcnt;
    wcnt += acf->PictureTotal * (sizeof(ShinePicture_t) - SHINE_PICTURE_ADDED_LENGTH);
    dprif("%d %d\n", acf->ElementAddr, acf->PictureAddr);

    for (i = 0; i < acf->ElementTotal; i++)
    {
        acf->ElementPtr[i].LocusOffset = wcnt;
        wcnt += acf->ElementPtr[i].LocusTotal * sizeof(ShineLocus_t);
        dprif("LocusOffset : %d, %d\n", acf->ElementPtr[i].LocusOffset, acf->ElementPtr[i].LocusOffset%4);
    }

    for (i = 0; i < acf->PictureTotal; i++)
    {
        acf->PicturePtr[i].PictureOffset = wcnt;
        FwritePicture(fp, acf->PicturePtr[i].PictureOffset, acf->PicturePtr[i].PictureContent, karma, &cnt);
        wcnt += cnt;
        dprif("PictureOffset : %d, %d\n", acf->PicturePtr[i].PictureOffset, acf->PicturePtr[i].PictureOffset%4);
    }

    area->WaterEdging.PictureOffset = wcnt;
    if (area->WaterEdging.Mode & WATER_EDGING_MODE_ENABLE)
    {
        FwriteWaterEdging(fp, area->WaterEdging.PictureOffset, &area->WaterEdging, karma, &cnt);
        wcnt += cnt;
    }

    if ((area->WaterEdging.Mode & WATER_EDGING_MODE_ENABLE) == 0)
    {
        memset(&area->WaterEdging, 0, sizeof(WaterEdging_t) - WATER_EDGING_ADDED_LENGTH);
    }

    fseek(fp, sizeof(FileHeader_t), SEEK_SET);
    cnt = fwrite(area, sizeof(uint16) + sizeof(Coord_t), 1, fp);
    cnt = fwrite(&area->WaterEdging, sizeof(WaterEdging_t) - WATER_EDGING_ADDED_LENGTH, 1, fp);

    fseek(fp, specific_para_addr, SEEK_SET);
    cnt = fwrite(acf, sizeof(ACFShine_t) - ACF_SHINE_ADDED_LENGTH, 1, fp);

    fseek(fp, acf->ElementAddr, SEEK_SET);
    for (i = 0; i < acf->ElementTotal; i++)
    {
        cnt = fwrite(&acf->ElementPtr[i], sizeof(ShineElement_t) - SHINE_ELEMENT_ADDED_LENGTH, 1, fp);
    }

    fseek(fp, acf->PictureAddr, SEEK_SET);
    for (i = 0; i < acf->PictureTotal; i++)
    {
        cnt = fwrite(&acf->PicturePtr[i], sizeof(ShinePicture_t) - SHINE_PICTURE_ADDED_LENGTH, 1, fp);
    }

    for (i = 0; i < acf->ElementTotal; i++)
    {
        fseek(fp, acf->ElementPtr[i].LocusOffset, SEEK_SET);
        cnt = fwrite(acf->ElementPtr[i].LocusPtr, sizeof(ShineLocus_t), acf->ElementPtr[i].LocusTotal, fp);
    }

    header.Version = FILE_VERSION;
    header.Length = wcnt;
    header.Reserved = 0;
    fseek(fp, 0, SEEK_SET);
    cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

    fclose(fp);

    dprif("CreatFile_ACFShine OK!\n");

    return 0;

out:
    record->ProgramErrorno = RECORD_ERRORNO_CREATE_FAIL;
    return -1;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int FreeMemory_ACFShine(Area_t *area, Record_t *record)
{
    ACFShine_t *acf;

    if (area == NULL)
    {
        dprif("FreeMemory_ACFShine parameter is NULL\n");
        return -1;
    }

    if (area->Type != AREA_SHINE)
    {
        dprif("FreeMemory_ACFShine type is error\n");
        return -2;
    }

    if (area->Ptr != NULL)
    {
        acf = area->Ptr;

        if (acf->PicturePtr != NULL)
        {
            free(acf->PicturePtr);
            acf->PicturePtr = NULL;
        }

        if (acf->ElementPtr != NULL)
        {
            int i;
            for (i = 0; i < acf->ElementTotal; i++)
            {
                if (acf->ElementPtr[i].LocusPtr != 0)
                {
                    free(acf->ElementPtr[i].LocusPtr);
                    acf->ElementPtr[i].LocusPtr = NULL;
                }
            }

            free(acf->ElementPtr);
            acf->ElementPtr = NULL;
        }

        free(area->Ptr);
        area->Ptr = NULL;
    }

    return 0;
}
