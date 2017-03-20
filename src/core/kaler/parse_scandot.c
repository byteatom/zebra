#include "kaler_config.h"
#include "../json/json.h"
#include "parse_scandot.h"

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
int JsonParse_ScanDotConfig(const json_object *new_obj, ScanDotConfig_t *scandot, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;

    int arraylen = 0, loop = 0;

    memset(scandot, 0, sizeof(ScanDotConfig_t));

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "model"))
        {
            strcpy(vstring, json_object_get_string(val));
            if (strlen(vstring) < SHORT_NAME_LEN)
            {
                dprif("Name : %s\n", vstring);
            }
            else
            {
                dprif("Name error,set name %s\n", vstring);
            }
        }
        else if (!strcmp(key, "width"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                scandot->Width = vint;
                dprif("Width : %d\n", scandot->Width);
            }
            else
            {
                dprif("Width error,current value %d\n", scandot->Width);
            }
        }
        else if (!strcmp(key, "emptyDot"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                scandot->EmptyDot = vint;
                if (vint == 0)
                    scandot->BMask &= ~SCANDOT_BMASK_BLANK_SCAN;
                else
                    scandot->BMask |= SCANDOT_BMASK_BLANK_SCAN;
                dprif("EmptyDot : %d\n", scandot->EmptyDot);
            }
            else
            {
                dprif("EmptyDot error,current value %d\n", scandot->EmptyDot);
            }
        }
        else if (!strcmp(key, "scanLine"))
        {
            vint = json_object_get_int(val);
            if ((vint <= 32) && (vint > 0))
            {
                scandot->ScanLine = vint;
                dprif("ScanLine : %d\n", scandot->ScanLine);
            }
            else
            {
                dprif("ScanLine error,current value %d\n", scandot->ScanLine);
            }
        }
        else if (!strcmp(key, "warpTimes"))
        {
            vint = json_object_get_int(val);
            if ((vint <= 32) && (vint > 0))
            {
                scandot->WarpTimes = vint;
                dprif("WarpTimes : %d\n", scandot->WarpTimes);
            }
            else
            {
                dprif("WarpTimes error,current value %d\n", scandot->WarpTimes);
            }
        }
        else if (!strcmp(key, "baseColor"))
        {
            vint = json_object_get_int(val);
            if ((vint == 1) || (vint == 2) || (vint == 3))
            {
                scandot->Gama = vint;
                dprif("Gama : %d\n", scandot->Gama);
            }
            else
            {
                dprif("Gama error,current value %d\n", scandot->Gama);
            }
        }
        else if (!strcmp(key, "abcdeMode"))
        {
            strcpy(vstring, json_object_get_string(val));

            if(!strcmp(vstring, "high138"))
                scandot->BMask &= ~SCANDOT_BMASK_ABCDE_MODE;
            else if(!strcmp(vstring, "highIO"))
                scandot->BMask |= SCANDOT_BMASK_ABCDE_MODE;
            else if(!strcmp(vstring, "low138"))
                scandot->BMask &= ~SCANDOT_BMASK_ABCDE_MODE;
            else if(!strcmp(vstring, "lowIO"))
                scandot->BMask |= SCANDOT_BMASK_ABCDE_MODE;
            else
            {
                dprif("abcdeMode key error\n");
            }

            dprif("BMask(abcdeMode) : %.4X\n", scandot->BMask);
        }
        else if (!strcmp(key, "dataPolar"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == FALSE)
                scandot->BMask |= SCANDOT_BMASK_DATA_POLAR;
            else
                scandot->BMask &= ~SCANDOT_BMASK_DATA_POLAR;

            dprif("BMask(dataPolar) : %.4X\n", scandot->BMask);
        }
        else if (!strcmp(key, "strPolar"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == FALSE)
                scandot->BMask &= ~SCANDOT_BMASK_STR_POLAR;
            else
                scandot->BMask &= ~SCANDOT_BMASK_STR_POLAR;

            dprif("BMask(strPolar) : %.4X\n", scandot->BMask);
        }
        else if (!strcmp(key, "oePolar"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == FALSE)
                scandot->BMask |= SCANDOT_BMASK_OE_POLAR;
            else
                scandot->BMask &= ~SCANDOT_BMASK_OE_POLAR;

            dprif("BMask(oePolar) : %.4X\n", scandot->BMask);
        }
        else if (!strcmp(key, "configIndex"))
        {
            scandot->ConfigIndex = json_object_get_int(val);
            dprif("ConfigIndex : %d\n", scandot->ConfigIndex);
        }
        else if (!strcmp(key, "dotMap"))
        {
            int i;
            char *ptmp;

            arraylen = json_object_array_length(val);

            if ((arraylen > SCANDOT_DOTMAP_LENGTH) || (arraylen < 0))
            {
                arraylen = 0;
                dprif("dotMap array number error set to 0");
            }
            dprif("dotMap arraylen %d\n", arraylen);

            for (i = 0; i < arraylen; i++)
            {
                json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
                {
                    if (!strcmp(key_1, "x"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            scandot->DotMap[i].x = vint;
                        }
                        else
                        {
                            dprif("DotMap[%d].x error value %d\n", i, vint);
                        }
                    }
                    else if (!strcmp(key_1, "y"))
                    {
                        vint = json_object_get_int(val_1);
                        if (vint >= 0)
                        {
                            scandot->DotMap[i].y = vint;
                        }
                        else
                        {
                            dprif("DotMap[%d].y error value %d\n", i, vint);
                        }
                    }
                }

                dprif("DotMap[%d] -> x:%d, y:%d\n", i, scandot->DotMap[i].x, scandot->DotMap[i].y);
            }
        }
        else if (!strcmp(key, "lineMap"))
        {
            int i;
            char *ptmp;
            int arraylen = json_object_array_length(val);

            if ((arraylen > SCANDOT_LINEMAP_LENGTH) || (arraylen < 0))
            {
                arraylen = 0;
                dprif("LineMap array number error set to 0");
            }
            dprif("LineMap arraylen %d\n", arraylen);

            for (i = 0; i < arraylen; i++)
            {
                vint = json_object_get_int(json_object_array_get_idx(val, i));
                if (vint >= 0)
                {
                    scandot->LineMap[i] = vint;
                }
                else
                {
                    dprif("LineMap[%d] error value %d\n", i, vint);
                }

                dprif("LineMap[%d] -> %d\n", i, scandot->LineMap[i]);
            }
        }
        else
        {
            dprif("ScreenConfig key \"%s\" can not recognized\n", key);
        }
    }

    if ((scandot->WarpTimes == 0) || (arraylen == 0))
    {
        record->ScanDotErrorno = RECORD_ERRORNO_KEYLACK;
        return -1;
    }

    if (arraylen < 256)
    {
        for (loop = arraylen; loop < 256; loop++)
        {
            scandot->DotMap[loop].x = scandot->DotMap[loop%arraylen].x + (loop/arraylen) * (arraylen/scandot->WarpTimes);
            scandot->DotMap[loop].y = scandot->DotMap[loop%arraylen].y;
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
int Inspect_ScanDotConfig(ScanDotConfig_t *scandot, Record_t *record)
{
    dprif("* Inspect_ScanDotConfig\n");

    dprif("ScanDotErrorno : %d\n", record->ScanDotErrorno);

    return record->ScanDotErrorno;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
extern const char ScanDotConfigTable[][SCANDOT_CONFIG_FILE_LENGTH];
int CreatFile_ScanDotConfig(const char *path, const ScanDotConfig_t *scandot, const ScreenConfig_t *screen, uint8 endian, Record_t *record)
{
    uint32 cnt;
    FILE *fp = NULL;
    FileHeader_t header;

    dprif("CreatFile_ScanDotConfig\n");

    if ((path == NULL) || (scandot == NULL))
    {
        dprif("CreatFile_ScanDotConfig parameter error\n");
        goto out;
    }

    if (path[strlen(path) - 1] != '/')
    {
        dprif("CreatFile_ScanDotConfig path error\n");
        goto out;
    }

    if (CreateDirectoryK(path) < 0)
        goto out;

    char file[PATH_LEN];

    if (endian == FILE_LITTLE_ENDIAN)
    {
        sprintf(file, "%s%s", path, "ALL.sef");
        memcpy(header.Sign, "SEF", 3);
    }
    else
    {
        sprintf(file, "%s%s", path, "ALL.sex");
        memcpy(header.Sign, "SEF", 3);
    }

    header.Version = FILE_VERSION;
    header.Length = sizeof(FileHeader_t) + sizeof(ScanDotConfig_t);
    header.Reserved = 0;

    if ((fp = fopen(file, "wb+")) == NULL)
    {
        dprif("fopen %s failed\n", file);
        goto out;
    }

    if ((screen->ScreenPath[0] != 0) && (screen->ScreenName[0] != 0))
    {
        dprif("CreatFile_ScanDotConfig from ScreenPath and ScreenName\n");
        FILE *fpscreen = NULL;
        char content[SCANDOT_CONFIG_FILE_LENGTH];
        char filepath[PATH_LEN + NAME_LEN];

        sprintf(filepath, "%s%s.SEF", screen->ScreenPath, screen->ScreenName);
        if ((fpscreen = fopen(filepath, "rb")) == NULL)
        {
            dprif("ScanDotContent fopen %s failed\n", screen->ScreenContent);
            fclose(fp);
            goto out;
        }
        fread(content, sizeof(char), SCANDOT_CONFIG_FILE_LENGTH, fpscreen);
        fclose(fpscreen);

        if(screen->BMask&SCREEN_BMASK_DATA_POLAR)
        {
            content[539] |= 1<<4;
            dprif("ScanDot DATA-\n");
        }
        else
        {
            content[539] &= ~(1<<4);
            dprif("ScanDot DATA+\n");
        }
        if(screen->BMask&SCREEN_BMASK_OE_POLAR)
        {
            content[539] |= 1<<6;
            dprif("ScanDot OE-\n");
        }
        else
        {
            content[539] &= ~(1<<6);
            dprif("ScanDot OE+\n");
        }
        content[539] &= ~(1<<3);

        dprif("ScanDot Bmask high8bit content[539] %x\n",content[539]);
        dprif("ScanDot Bmask low8bit content[540] %x\n",content[540]);

        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(content, sizeof(char), SCANDOT_CONFIG_FILE_LENGTH, fp);
    }
    else if (screen->ScanDotContent[0] != 0)
    {
        dprif("CreatFile_ScanDotConfig from ScanDotContent\n");
        FILE *fpscreen = NULL;
        char content[SCANDOT_CONFIG_FILE_LENGTH];

        if ((fpscreen = fopen(screen->ScanDotContent, "rb")) == NULL)
        {
            dprif("ScanDotContent fopen %s failed\n", screen->ScreenContent);
            fclose(fp);
            goto out;
        }
        fread(content, sizeof(char), SCANDOT_CONFIG_FILE_LENGTH, fpscreen);
        fclose(fpscreen);

        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(content, sizeof(char), SCANDOT_CONFIG_FILE_LENGTH, fp);
    }
    else if (screen->ConfigIndex == 0)
    {
        dprif("CreatFile_ScanDotConfig from parameter\n");
        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

        fseek(fp, sizeof(FileHeader_t), SEEK_SET);
        cnt = fwrite(scandot, sizeof(ScanDotConfig_t), 1, fp);
    }
    else if (screen->ConfigIndex < SCREEN_FILE_MAX)
    {
        dprif("CreatFile_ScanDotConfig from ConfigIndex\n");
        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(ScanDotConfigTable[screen->ConfigIndex], sizeof(char), SCANDOT_CONFIG_FILE_LENGTH, fp);
    }
    else
    {
        fclose(fp);
        goto out;
    }

    fclose(fp);

    dprif("CreatFile_ScanDotConfig OK!\n");

    return 0;

out:
    dprif("CreatFile_ScanDotConfig fail!\n");
    record->ScanDotErrorno = RECORD_ERRORNO_CREATE_FAIL;
    return -1;
}

