#include "kaler_config.h"
#include "../json/json.h"
#include "parse_acfwatch.h"

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
int JsonParse_ACFWatchConfig(const json_object *new_obj, ACFWatch_t **acf, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;
    ACFWatch_t *pacf;

    *acf = malloc(sizeof(ACFWatch_t));
    memset(*acf, 0, sizeof(ACFWatch_t));
    
    pacf = *acf;

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "jetLag"))
        {
            char *ptmp;
            uint8 hourtmp;

            strcpy(vstring, json_object_get_string(val));
            ptmp = strtok(vstring, ":");//
            if (ptmp != NULL)
            {
                hourtmp = (uint16)atoi((char*)ptmp);
                if (hourtmp > 0)
                {
                    pacf->JetLag[0] = 1;
                    pacf->JetLag[1] = hourtmp;
                }
                else
                {
                    pacf->JetLag[0] = 0;
                    pacf->JetLag[1] = abs(hourtmp);
                }
            }
            ptmp = strtok(NULL, ":");//
            if (ptmp != NULL) pacf->JetLag[2] = (uint16)atoi((char*)ptmp);
            ptmp =strtok(NULL, "\0");//
            if (ptmp != NULL) pacf->JetLag[3] = (uint16)atoi((char*)ptmp);
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
        else if (!strcmp(key, "hourHandArray"))
        {
            int i;
            int arraylen = json_object_array_length((json_object *)val);

            if (arraylen != 60)
            {
                arraylen = 0;
                dprif("hourHandArray number error set to 0\n");
            }

            dprif("hourHandArray arraylen %d\n", arraylen);
            for (i = 0; i < arraylen; i++)
            {
                json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
                {
                    if (!strcmp(key_1, "startX"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->HourHand[i].X = vint;
                            dprif("HourHand[%d].X : %d\n", i, pacf->HourHand[i].X);
                        }
                        else
                        {
                            dprif("HourHand[%d].X error,current value %d\n", i, pacf->HourHand[i].X);
                        }
                    }
                    else if (!strcmp(key_1, "startY"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->HourHand[i].Y = vint;
                            dprif("HourHand[%d].Y : %d\n", i, pacf->HourHand[i].Y);
                        }
                        else
                        {
                            dprif("HourHand[%d].Y error,current value %d\n", i, pacf->HourHand[i].Y);
                        }
                    }
                    else if (!strcmp(key_1, "width"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->HourHand[i].Width = vint;
                            dprif("HourHand[%d].Width : %d\n", i, pacf->HourHand[i].Width);
                        }
                        else
                        {
                            dprif("HourHand[%d].Width error,current value %d\n", i, pacf->HourHand[i].Width);
                        }
                    }
                    else if (!strcmp(key_1, "height"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->HourHand[i].Height = vint;
                            dprif("HourHand[%d].Height : %d\n", i, pacf->HourHand[i].Height);
                        }
                        else
                        {
                            dprif("HourHand[%d].Height error,current value %d\n", i, pacf->HourHand[i].Height);
                        }
                    }
                    else if (!strcmp(key_1, "content"))
                    {  
                        strcpy(vstring, json_object_get_string(val_1));
                        if (strlen(vstring) < PATH_LEN)
                        {
                            strcpy(pacf->HourHandContent[i], vstring);
                            dprif("HourHandContent[%d] : %s\n", i, pacf->HourHandContent[i]);
                        }
                        else
                        {
                            dprif("HourHandContent[%d] critical error\n", i);
                        }
                    }
                }
            }
        }
        else if (!strcmp(key, "minHandArray"))
        {
            int i;
            int arraylen = json_object_array_length((json_object *)val);

            if (arraylen != 60)
            {
                arraylen = 0;
                dprif("minHandArray array number error set to 0\n");
            }

            dprif("minHandArray arraylen %d\n", arraylen);
            for (i = 0; i < arraylen; i++)
            {
                json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
                {
                    if (!strcmp(key_1, "startX"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->MinHand[i].X = vint;
                            dprif("MinHand[%d].X : %d\n", i, pacf->MinHand[i].X);
                        }
                        else
                        {
                            dprif("MinHand[%d].X error,current value %d\n", i, pacf->MinHand[i].X);
                        }
                    }
                    else if (!strcmp(key_1, "startY"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->MinHand[i].Y = vint;
                            dprif("MinHand[%d].Y : %d\n", i, pacf->MinHand[i].Y);
                        }
                        else
                        {
                            dprif("MinHand[%d].Y error,current value %d\n", i, pacf->MinHand[i].Y);
                        }
                    }
                    else if (!strcmp(key_1, "width"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->MinHand[i].Width = vint;
                            dprif("MinHand[%d].Width : %d\n", i, pacf->MinHand[i].Width);
                        }
                        else
                        {
                            dprif("MinHand[%d].Width error,current value %d\n", i, pacf->MinHand[i].Width);
                        }
                    }
                    else if (!strcmp(key_1, "height"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->MinHand[i].Height = vint;
                            dprif("MinHand[%d].Height : %d\n", i, pacf->MinHand[i].Height);
                        }
                        else
                        {
                            dprif("MinHand[%d].Height error,current value %d\n", i, pacf->MinHand[i].Height);
                        }
                    }
                    else if (!strcmp(key_1, "content"))
                    {
                        strcpy(vstring, json_object_get_string(val_1));
                        if (strlen(vstring) < PATH_LEN)
                        {
                            strcpy(pacf->MinHandContent[i], vstring);
                            dprif("MinHandContent[%d] : %s\n", i, pacf->MinHandContent[i]);
                        }
                        else
                        {
                            dprif("MinHandContent[%d] critical error\n", i);
                        }
                    }
                }
            }
        }
        else if (!strcmp(key, "secHandArray"))
        {
            int i;
            int arraylen = json_object_array_length((json_object *)val);

            if (arraylen != 60)
            {
                arraylen = 0;
                dprif("secHandArray array number error set to 0\n");
            }

            dprif("secHandArray arraylen %d\n", arraylen);
            for (i = 0; i < arraylen; i++)
            {
                json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
                {
                    if (!strcmp(key_1, "startX"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->SecHand[i].X = vint;
                            dprif("SecHand[%d].X : %d\n", i, pacf->SecHand[i].X);
                        }
                        else
                        {
                            dprif("SecHand[%d].X error,current value %d\n", i, pacf->SecHand[i].X);
                        }
                    }
                    else if (!strcmp(key_1, "startY"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->SecHand[i].Y = vint;
                            dprif("SecHand[%d].Y : %d\n", i, pacf->SecHand[i].Y);
                        }
                        else
                        {
                            dprif("SecHand[%d].Y error,current value %d\n", i, pacf->SecHand[i].Y);
                        }
                    }
                    else if (!strcmp(key_1, "width"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->SecHand[i].Width = vint;
                            dprif("SecHand[%d].Width : %d\n", i, pacf->SecHand[i].Width);
                        }
                        else
                        {
                            dprif("SecHand[%d].Width error,current value %d\n", i, pacf->SecHand[i].Width);
                        }
                    }
                    else if (!strcmp(key_1, "height"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            pacf->SecHand[i].Height = vint;
                            dprif("SecHand[%d].Height : %d\n", i, pacf->SecHand[i].Height);
                        }
                        else
                        {
                            dprif("SecHand[%d].Height error,current value %d\n", i, pacf->SecHand[i].Height);
                        }
                    }
                    else if (!strcmp(key_1, "content"))
                    {
                        strcpy(vstring, json_object_get_string(val_1));
                        if (strlen(vstring) < PATH_LEN)
                        {
                            strcpy(pacf->SecHandContent[i], vstring);
                            dprif("SecHandContent[%d] : %s\n", i, pacf->SecHandContent[i]);
                        }
                        else
                        {
                            dprif("SecHandContent[%d] critical error\n", i);
                        }
                    }
                }
            }
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
int CreatFile_ACFWatch(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record)
{
    int i;
    uint32 cnt;
    uint32 modelcnt;
    FILE *fp = NULL;
    FileHeader_t header;
    uint32 wcnt = 0;
    
    dprif("CreatFile_ACFWatch\n");

	if ((path == NULL) || (area == NULL))
	{
		dprif("CreatFile_ACFWatch parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ACFWatch path error\n");
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
    ACFWatch_t *acf = area->Ptr;
    uint32 specific_para_addr;
    //uint32 picture_para_addr;

    wcnt += sizeof(FileHeader_t) + sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
    specific_para_addr = wcnt;
    acf->ParameterOffset = specific_para_addr + 8;

    wcnt += sizeof(ACFWatch_t) - ACF_WATCH_ADDED_LENGTH;
    area->PictureAddr = wcnt;
    FwritePicture(fp, area->PictureAddr, acf->BackgroundContent, karma, &cnt);

    wcnt += cnt;
    for (i = 0; i < 60; i++)
    {
        acf->HourHand[i].PictureOffset = wcnt;
        FwritePicture(fp, acf->HourHand[i].PictureOffset, acf->HourHandContent[i], karma, &cnt);
        wcnt += cnt;
    }

    for (i = 0; i < 60; i++)
    {
        acf->MinHand[i].PictureOffset = wcnt;
        FwritePicture(fp, acf->MinHand[i].PictureOffset, acf->MinHandContent[i], karma, &cnt);
        wcnt += cnt;
    }

    for (i = 0; i < 60; i++)
    {
        acf->SecHand[i].PictureOffset = wcnt;
        FwritePicture(fp, acf->SecHand[i].PictureOffset, acf->SecHandContent[i], karma, &cnt);
        wcnt += cnt;
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
    cnt = fwrite(area, sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH), 1, fp);

    fseek(fp, specific_para_addr, SEEK_SET);
    cnt = fwrite(acf, sizeof(ACFWatch_t) - ACF_WATCH_ADDED_LENGTH, 1, fp);

    header.Version = FILE_VERSION;
    header.Length = wcnt;
    header.Reserved = 0;
    fseek(fp, 0, SEEK_SET);
    cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

    fclose(fp);

    dprif("CreatFile_ACFWatch OK!\n");

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
int FreeMemory_ACFWatch(Area_t *area, Record_t *record)
{
    if (area == NULL)
    {
        dprif("FreeMemory_ACFWatch parameter is NULL\n");
        return -1;
    }

    if (area->Type != AREA_WATCH)
    {
        dprif("FreeMemory_ACFWatch type is error\n");
        return -2;
    }

    if (area->Ptr != NULL)
    {
        free(area->Ptr);
        area->Ptr = NULL;
    }

    return 0;
}

