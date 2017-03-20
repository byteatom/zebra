#include "kaler_config.h"
#include "../json/json.h"
#include "parse_acfword.h"

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
int JsonParse_ACFWordPictureFuseArray(const json_object *new_obj, PictureFuse_t **picfuse, PictureExtend_t **picext, uint16 *number);
int JsonParse_ACFWordConfig(const json_object *new_obj, ACFWord_t **acf, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;
    ACFWord_t *pacf;

    *acf = malloc(sizeof(ACFWord_t));
    memset(*acf, 0, sizeof(ACFWord_t));
    
    pacf = *acf;

    {// first read picture total
        json_object_object_foreach((json_object *)new_obj, key, val)
        {
            if (!strcmp(key, "pictureTotal"))
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
        }
    }
    
    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "pictureFuseArray"))
        {
            JsonParse_ACFWordPictureFuseArray(val, 
                                        &pacf->PictureFusePtr,
                                        &pacf->PictureExtendPtr,
                                        &pacf->PictureTotal);
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
int JsonParse_ACFWordPictureFuseArray(const json_object *new_obj, PictureFuse_t **picfuse, PictureExtend_t **picext, uint16 *number)
{
    PictureFuse_t *pfuse;
    PictureExtend_t *pext;
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;
    int i;
    int arraylen = json_object_array_length((json_object *)new_obj);

    if (arraylen < 0)
    {
        arraylen = 0;
        dprif("ACFPicture array number error set to 0");
    }

    *number = arraylen;

    *picfuse = malloc(sizeof(PictureFuse_t)*arraylen);
    memset(*picfuse, 0, sizeof(PictureFuse_t)*arraylen);

    *picext = malloc(sizeof(PictureExtend_t)*arraylen);
    memset(*picext, 0, sizeof(PictureExtend_t)*arraylen);

    pfuse = *picfuse;
    pext = *picext;

    for (i = 0; i < arraylen; i++)
    {
        dprif("----> ACFPicture : %d\n", i);

        json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
        {
            if (!strcmp(key, "enterMode"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pfuse[i].EnterMode = vint;
                    dprif("EnterMode : %d\n", pfuse[i].EnterMode);
                }
                else
                {
                    dprif("EnterMode error,current value %d\n", pfuse[i].EnterMode);
                }
            }
            else if (!strcmp(key, "exitMode"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pfuse[i].ExitMode = vint;
                    dprif("ExitMode : %d\n", pfuse[i].ExitMode);
                }
                else
                {
                    dprif("ExitMode error,current value %d\n", pfuse[i].ExitMode);
                }
            }
            else if (!strcmp(key, "speed"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pfuse[i].Speed = vint;
                    dprif("Speed : %d\n", pfuse[i].Speed);
                }
                else
                {
                    dprif("Speed error,current value %d\n", pfuse[i].Speed);
                }
            }
            else if (!strcmp(key, "delay"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pfuse[i].Delay = vint;
                    dprif("Delay : %d\n", pfuse[i].Delay);
                }
                else
                {
                    dprif("Delay error,current value %d\n", pfuse[i].Delay);
                }
            }
            else if (!strcmp(key, "jumpGrid"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pfuse[i].JumpGrid = vint;
                    dprif("JumpGrid : %d\n", pfuse[i].JumpGrid);
                }
                else
                {
                    dprif("JumpGrid error,current value %d\n", pfuse[i].JumpGrid);
                }
            }
            else if (!strcmp(key, "width"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pext[i].Width = vint;
                    dprif("Width : %d\n", pext[i].Width);
                }
                else
                {
                    dprif("Width error,current value %d\n", pext[i].Width);
                }
            }
            else if (!strcmp(key, "height"))
            {
                vint = json_object_get_int(val);
                if (vint >= 0)
                {
                    pext[i].Height = vint;
                    dprif("Height : %d\n", pext[i].Height);
                }
                else
                {
                    dprif("Height error,current value %d\n", pext[i].Height);
                }
            }
            else if (!strcmp(key, "content"))
            {
                strcpy(vstring, json_object_get_string(val));
                if (strlen(vstring) < PATH_LEN)
                {
                    strcpy(pfuse[i].PictureContent, vstring);
                    dprif("PictureContent : %s\n", pfuse[i].PictureContent);
                }
                else
                {
                    dprif("PictureContent critical error\n");
                }
            }
            else
            {
                dprif("ACFWordConfig key \"%s\" can not recognized\n", key);
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
int CreatFile_ACFWord(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record)
{
    int i;
    uint32 cnt;
    FILE *fp = NULL;
    FileHeader_t header;
    uint32 wcnt = 0;
    
    dprif("CreatFile_ACFWord\n");

	if ((path == NULL) || (area == NULL))
	{
		dprif("CreatFile_ACFWord parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ACFWord path error\n");
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
    ACFWord_t *acf = area->Ptr;
    uint32 specific_para_addr;
    uint32 picture_para_addr;

    wcnt += sizeof(FileHeader_t) + sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
    specific_para_addr = wcnt;

    wcnt += sizeof(ACFWord_t) - ACF_WORD_ADDED_LENGTH;
    picture_para_addr = wcnt;
    area->PictureAddr = wcnt;

    wcnt += acf->PictureTotal * (sizeof(PictureFuse_t) - PICTURE_FUSE_ADDED_LENGTH);
    for (i = 0; i < acf->PictureTotal; i++)
    {
        acf->PictureFusePtr[i].PictureOffset = wcnt;
        FwritePicture(fp, acf->PictureFusePtr[i].PictureOffset, acf->PictureFusePtr[i].PictureContent, karma, &cnt);
        wcnt += cnt;
        fseek(fp, wcnt, SEEK_SET);
        cnt = fwrite(&acf->PictureExtendPtr[i], sizeof(PictureExtend_t), 1, fp);
        //dprif("%d %d\n", acf->PictureExtendPtr[i].Width, acf->PictureExtendPtr[i].Height);
        wcnt += sizeof(PictureExtend_t);
        fseek(fp, wcnt, SEEK_SET);
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
    cnt = fwrite(acf, sizeof(ACFWord_t) - ACF_WORD_ADDED_LENGTH, 1, fp);

    fseek(fp, picture_para_addr, SEEK_SET);
    for (i = 0; i < acf->PictureTotal; i++)
    {
        cnt = fwrite(&acf->PictureFusePtr[i], sizeof(PictureFuse_t) - PICTURE_FUSE_ADDED_LENGTH, 1, fp);
    }

    header.Version = FILE_VERSION;
    header.Length = wcnt;
    header.Reserved = 0;
    fseek(fp, 0, SEEK_SET);
    cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

    fclose(fp);

    dprif("CreatFile_ACFWord OK!\n");

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
int FreeMemory_ACFWord(Area_t *area, Record_t *record)
{   
    ACFWord_t *acf;

    if (area == NULL)
    {
        dprif("FreeMemory_ACFWord parameter is NULL\n");
        return -1;
    }

    if (area->Type != AREA_WORD)
    {
        dprif("FreeMemory_ACFWord type is error\n");
        return -2;
    }

    if (area->Ptr != NULL)
    {
        acf = area->Ptr;

        if (acf->PictureExtendPtr != NULL)
        {
            free(acf->PictureExtendPtr);
            acf->PictureExtendPtr = NULL;
        }

        if (acf->PictureFusePtr != NULL)
        {
            free(acf->PictureFusePtr);
            acf->PictureFusePtr = NULL;
        }

        free(area->Ptr);
        area->Ptr = NULL;
    }

    return 0;
}
