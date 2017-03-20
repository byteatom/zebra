#include "kaler_config.h"
#include "../json/json.h"
#include "parse_acfcount.h"

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
int JsonParse_ACFCountConfig(const json_object *new_obj, ACFCount_t **acf, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;
    ACFCount_t *pacf;

    *acf = malloc(sizeof(ACFCount_t));
    memset(*acf, 0, sizeof(ACFCount_t));

    pacf = *acf;

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "dayWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->DayWidth = vint;
                dprif("DayWidth : %d\n", pacf->DayWidth);
            }
            else
            {
                dprif("DayWidth error,current value %d\n", pacf->DayWidth);
            }
        }
        else if (!strcmp(key, "hourWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->HourWidth = vint;
                dprif("HourWidth : %d\n", pacf->HourWidth);
            }
            else
            {
                dprif("HourWidth error,current value %d\n", pacf->HourWidth);
            }
        }
        else if (!strcmp(key, "minWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->MinWidth = vint;
                dprif("MinWidth : %d\n", pacf->MinWidth);
            }
            else
            {
                dprif("MinWidth error,current value %d\n", pacf->MinWidth);
            }
        }
        else if (!strcmp(key, "secWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->SecWidth = vint;
                dprif("SecWidth : %d\n", pacf->SecWidth);
            }
            else
            {
                dprif("SecWidth error,current value %d\n", pacf->SecWidth);
            }
        }
        else if (!strcmp(key, "backgroundContent"))
        {
            strcpy(vstring, json_object_get_string(val));
            if (strlen(vstring) < PATH_LEN)
            {
                strcpy(pacf->BackgroundContent, vstring);
                dprif("BackgroundContent : %s\n", pacf->BackgroundContent);
            }
            else
            {
                dprif("BackgroundContent critical error\n");
            }
        }
        else if (!strcmp(key, "showDay"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                pacf->BMask |= ACF_COUNT_SHOW_DAY;
            else
                pacf->BMask &= ~ACF_COUNT_SHOW_DAY;

            dprif("BMask(showDay) : %.4X\n", pacf->BMask);
        }
        else if (!strcmp(key, "showHour"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                pacf->BMask |= ACF_COUNT_SHOW_HOUR;
            else
                pacf->BMask &= ~ACF_COUNT_SHOW_HOUR;

            dprif("BMask(showHour) : %.4X\n", pacf->BMask);
        }
        else if (!strcmp(key, "showMin"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                pacf->BMask |= ACF_COUNT_SHOW_MIN;
            else
                pacf->BMask &= ~ACF_COUNT_SHOW_MIN;

            dprif("BMask(showMin) : %.4X\n", pacf->BMask);
        }
        else if (!strcmp(key, "showSec"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                pacf->BMask |= ACF_COUNT_SHOW_SEC;
            else
                pacf->BMask &= ~ACF_COUNT_SHOW_SEC;

            dprif("BMask(showSec) : %.4X\n", pacf->BMask);
        }
        else if (!strcmp(key, "timingMode"))
        {
            /*
            strcpy(vstring, json_object_get_string(val));
            if(!strcmp(vstring, "up"))
                pacf->BMask |= ACF_COUNT_EN_TURN;
            else if(!strcmp(vstring, "down"))
                pacf->BMask &= ~ACF_COUNT_EN_TURN;
            else
            {
                dprif("BMask(timingMode) key error\n");
            }

            dprif("BMask(timingMode) : %.4X\n", pacf->BMask);
            */
        }
        else if (!strcmp(key, "enTurn"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                pacf->BMask |= ACF_COUNT_EN_TURN;
            else
                pacf->BMask &= ~ACF_COUNT_EN_TURN;

            dprif("BMask(enTurn) : %.4X\n", pacf->BMask);
        }
        else if (!strcmp(key, "digitFontWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->DigitFontWidth = vint;
                dprif("DigitFontWidth : %d\n", pacf->DigitFontWidth);
            }
            else
            {
                dprif("DigitFontWidth error,current value %d\n", pacf->DigitFontWidth);
            }
        }
        else if (!strcmp(key, "digitFontHeight"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->DigitFontHeight = vint;
                dprif("DigitFontHeight : %d\n", pacf->DigitFontHeight);
            }
            else
            {
                dprif("DigitFontHeight error,current value %d\n", pacf->DigitFontHeight);
            }
        }
        else if (!strcmp(key, "digitPictureWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->DigitPictureWidth = ((vint+15)/16) * 16;
                dprif("DigitPictureWidth : %d\n", pacf->DigitPictureWidth);
            }
            else
            {
                dprif("DigitPictureWidth error,current value %d\n", pacf->DigitPictureWidth);
            }
        }
        else if (!strcmp(key, "digitPictureHeight"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->DigitPictureHeight = vint;
                dprif("DigitPictureHeight : %d\n", pacf->DigitPictureHeight);
            }
            else
            {
                dprif("DigitPictureHeight error,current value %d\n", pacf->DigitPictureHeight);
            }
        }
        else if (!strcmp(key, "digitX"))
        {
            vint = json_object_get_int(val);
            pacf->DigitX = vint;
            dprif("DigitX : %d\n", pacf->DigitX);
        }
        else if (!strcmp(key, "digitY"))
        {
            vint = json_object_get_int(val);
            pacf->DigitY = vint;
            dprif("DigitY : %d\n", pacf->DigitY);
        }
        else if (!strcmp(key, "digitContentArray"))
        {
            int i;
            int arraylen = json_object_array_length((json_object *)val);

            if (arraylen != 10)
            {
                arraylen = 0;
                dprif("digitContent array number error set to 0\n");
            }

            dprif("digitContent arraylen %d\n", arraylen);
            for (i = 0; i < arraylen; i++)
            {
                strcpy(vstring, json_object_get_string(json_object_array_get_idx(val, i)));
                if (strlen(vstring) < PATH_LEN)
                {
                    strcpy(pacf->DigitContent[i], vstring);
                    dprif("DigitContent[%d] : %s\n", i, pacf->DigitContent[i]);
                }
                else
                {
                    dprif("DigitContent[%d] critical error\n", i);
                }
            }
        }
        else if (!strcmp(key, "secondPictureWidth"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->SecondPictureWidth = vint;
                dprif("SecondPictureWidth : %d\n", pacf->SecondPictureWidth);
            }
            else
            {
                dprif("SecondPictureWidth error,current value %d\n", pacf->SecondPictureWidth);
            }
        }
        else if (!strcmp(key, "secondPictureHeight"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                pacf->SecondPictureHeight = vint;
                dprif("SecondPictureHeight : %d\n", pacf->SecondPictureHeight);
            }
            else
            {
                dprif("SecondPictureHeight error,current value %d\n", pacf->SecondPictureHeight);
            }
        }
        else if (!strcmp(key, "secondContent"))
        {
            strcpy(vstring, json_object_get_string(val));
            if (strlen(vstring) < PATH_LEN)
            {
                strcpy(pacf->SecondContent, vstring);
                dprif("SecondContent : %s\n", pacf->SecondContent);
            }
            else
            {
                dprif("SecondContent critical error\n");
            }
        }
        else if (!strcmp(key, "aimTime"))
        {
            JsonParse_DateTime(val, &pacf->AimTime, PARSE_WEEK_MODE_NUMBER);
            dprif("AimTime date : %d-%d-%d\n", pacf->AimTime.Year, pacf->AimTime.Month, pacf->AimTime.Day);
            dprif("AimTime week : %d\n", pacf->AimTime.Week);
            dprif("AimTime time : %d:%d:%d\n", pacf->AimTime.Hour, pacf->AimTime.Min, pacf->AimTime.Sec);
        }
        else
        {
            dprif("ACFCountConfig key \"%s\" can not recognized\n", key);
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
int CreatFile_ACFCount(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record)
{
    int i;
    uint32 cnt;
    uint32 modelcnt;
    FILE *fp = NULL;
    FileHeader_t header;
    uint32 wcnt = 0;

    dprif("CreatFile_ACFCount\n");

    if ((path == NULL) || (area == NULL))
    {
        dprif("CreatFile_ACFCount parameter error\n");
        goto out;
    }

    if (path[strlen(path) - 1] != '/')
    {
        dprif("CreatFile_ACFCount path error\n");
        goto out;
    }

    if (CreateDirectoryK(path) < 0)
        goto out;

    char file[PATH_LEN];

    sprintf(file, "%s%s", path, area->Name);
    memcpy(header.Sign, "ACF", 3);

    if ((fp = fopen(file, "wb+")) == NULL)
    {
        dprif("fopen %s failed\n", file);
        goto out;
    }

    //-----------------------------------------------------------------------//
    ACFCount_t *acf = area->Ptr;
    uint32 specific_para_addr;

    wcnt += sizeof(FileHeader_t) + sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
    specific_para_addr = wcnt;

    wcnt += sizeof(ACFCount_t) - ACF_COUNT_ADDED_LENGTH;
    area->PictureAddr = wcnt;
    FwritePicture(fp, area->PictureAddr, acf->BackgroundContent, karma, &cnt);

    wcnt += cnt;
    acf->SecondContentOffset = wcnt;
    FwritePicture(fp, acf->SecondContentOffset, acf->SecondContent, karma, &cnt);

    wcnt += cnt;
    acf->DigitContentOffset = wcnt;
    modelcnt = 0;
    for (i = 0; i < 10; i++)
    {
        FwritePicture(fp, acf->DigitContentOffset + modelcnt, acf->DigitContent[i], karma, &cnt);
        modelcnt += cnt;
    }

    wcnt += modelcnt;
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
    cnt = fwrite(area, sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH), 1, fp);

    fseek(fp, specific_para_addr, SEEK_SET);
    cnt = fwrite(acf, sizeof(ACFCount_t) - ACF_COUNT_ADDED_LENGTH, 1, fp);

    header.Version = FILE_VERSION;
    header.Length = wcnt;
    header.Reserved = 0;
    fseek(fp, 0, SEEK_SET);
    cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

    fclose(fp);

    dprif("CreatFile_ACFCount OK!\n");

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
int FreeMemory_ACFCount(Area_t *area, Record_t *record)
{
    //ACFTime_t *acf;

    if (area == NULL)
    {
        dprif("FreeMemory_ACFCount parameter is NULL\n");
        return -1;
    }

    if (area->Type != AREA_COUNT)
    {
        dprif("FreeMemory_ACFCount type is error\n");
        return -2;
    }

    if (area->Ptr != NULL)
    {
        free(area->Ptr);
        area->Ptr = NULL;
    }

    return 0;
}

