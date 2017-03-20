
#include "kaler_config.h"
#include "../json/json.h"
#include "parse_screen.h"

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
int JsonParse_ScreenConfig(const json_object *new_obj, ScreenConfig_t *screen, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;
    double gama;

    memset(screen, 0, sizeof(ScreenConfig_t));

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "model"))
        {
            strcpy(vstring, json_object_get_string(val));
            if (strlen(vstring) < SHORT_NAME_LEN)
            {
                strcpy(screen->Model, vstring);
                dprif("Name : %s\n", screen->Model);
            }
            else
            {
                strcpy(screen->Model, "none");
                dprif("Name error,set name %s\n", screen->Model);
            }
        }
        else if (!strcmp(key, "screenContent"))
        {
            strcpy(vstring, json_object_get_string(val));
            if ((strlen(vstring) < PATH_LEN) && (strlen(vstring) != 0))
            {
                strcpy(screen->ScreenContent, vstring);
                dprif("ScreenContent : %s\n", screen->ScreenContent);
            }
        }
        else if (!strcmp(key, "scanDotContent"))
        {
            strcpy(vstring, json_object_get_string(val));
            if ((strlen(vstring) < PATH_LEN) && (strlen(vstring) != 0))
            {
                strcpy(screen->ScanDotContent, vstring);
                dprif("ScanDotContent : %s\n", screen->ScanDotContent);
            }
        }
        else if (!strcmp(key, "screenPath"))
        {
            strcpy(vstring, json_object_get_string(val));
            if ((strlen(vstring) < PATH_LEN) && (strlen(vstring) != 0))
            {
                strcpy(screen->ScreenPath, vstring);
                if (screen->ScreenPath[strlen(screen->ScreenPath) - 1] != '/')
                {
                    screen->ScreenPath[strlen(screen->ScreenPath) + 0] = '/';
                    screen->ScreenPath[strlen(screen->ScreenPath) + 1] = 0;
                }
                dprif("ScreenPath : %s\n", screen->ScreenPath);
            }
        }
        else if (!strcmp(key, "screenName"))
        {
            strcpy(vstring, json_object_get_string(val));
            if ((strlen(vstring) < NAME_LEN) && (strlen(vstring) != 0))
            {
                strcpy(screen->ScreenName, vstring);
                dprif("ScreenName : %s\n", screen->ScreenName);
            }
        }
        else if (!strcmp(key, "width"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                screen->Width = vint;
                dprif("Width : %d\n", screen->Width);
            }
            else
            {
                dprif("Width error,current value %d\n", screen->Width);
            }
        }
        else if (!strcmp(key, "height"))
        {
            vint = json_object_get_int(val);
            if (vint >= 0)
            {
                screen->Height = vint;
                dprif("Height : %d\n", screen->Height);
            }
            else
            {
                dprif("Height error,current value %d\n", screen->Height);
            }
        }
        else if (!strcmp(key, "scanLine"))
        {
            vint = json_object_get_int(val);
            if ((vint <= 32) && (vint > 0))
            {
                screen->ScanLine = vint;
                dprif("ScanLine : %d\n", screen->ScanLine);
            }
            else
            {
                dprif("ScanLine error,current value %d\n", screen->ScanLine);
            }
        }
        else if (!strcmp(key, "scanMode"))
        {
            vint = json_object_get_int(val);
            if ((vint <= 32) && (vint > 0))
            {
                screen->ScanMode = vint;
                dprif("ScanMode : %d\n", screen->ScanMode);
            }
            else
            {
                dprif("ScanMode error,current value %d\n", screen->ScanMode);
            }
        }
        else if (!strcmp(key, "baseColor"))
        {
            vint = json_object_get_int(val);
            if ((vint == 1) || (vint == 2) || (vint == 3))
            {
                screen->BaseColor |= vint;
                dprif("BaseColor : %d\n", screen->BaseColor);
            }
            else
            {
                dprif("BaseColor error,current value %d\n", screen->BaseColor);
            }
        }
        else if (!strcmp(key, "gama"))
        {
            gama = json_object_get_double(val);
            if (gama > 0)
                dprif("Gama : %f\n", gama);
            else
                dprif("Gama error,current value %f\n", gama);
        }
        else if (!strcmp(key, "warpBit"))
        {
            vint = json_object_get_int(val);
            if ((vint == 0) || (vint == 4) || (vint == 8) || (vint == 16))
            {
                screen->WarpBit = vint;
                dprif("WarpBit : %d\n", screen->WarpBit);
            }
            else
            {
                dprif("WarpBit error,current value %d\n", screen->WarpBit);
            }
        }
        else if (!strcmp(key, "warpMode"))
        {
            strcpy(vstring, json_object_get_string(val));

            if(!strcmp(vstring, "upWarp"))
                screen->BMask |= SCREEN_BMASK_WARP_MODE;
            else if(!strcmp(vstring, "downWarp"))
                screen->BMask &= ~SCREEN_BMASK_WARP_MODE;
            else if(!strcmp(vstring, "straight"))
                dprif("warpMode straight\n");
            else
            {
                dprif("warpMode key error\n");
            }

            dprif("BMask(warpMode) : %.4X\n", screen->BMask);
        }
        else if (!strcmp(key, "abcdeMode"))
        {
            strcpy(vstring, json_object_get_string(val));

            if(!strcmp(vstring, "high138"))
                screen->ABCDEMode = 0;
            else if(!strcmp(vstring, "highIO"))
                screen->ABCDEMode = 1;
            else if(!strcmp(vstring, "low138"))
                screen->ABCDEMode = 2;
            else if(!strcmp(vstring, "lowIO"))
                screen->ABCDEMode = 3;
            else
            {
                dprif("ABCDEMode key error\n");
            }

            dprif("ABCDEMode : %d\n", screen->ABCDEMode);
        }
        else if (!strcmp(key, "dataPolar"))
        {
            vbool = json_object_get_boolean(val);
            dprif("dataPolar : %d\n", vbool);
            if (vbool == FALSE)
                screen->BMask |= SCREEN_BMASK_DATA_POLAR;
            else
                screen->BMask &= ~SCREEN_BMASK_DATA_POLAR;

            dprif("BMask(dataPolar) : %.4X\n", screen->BMask);
        }
        else if (!strcmp(key, "strPolar"))
        {
            // STR
            vbool = json_object_get_boolean(val);
            if (vbool == FALSE)
                screen->BMask &= ~SCREEN_BMASK_STR_POLAR;
            else
                screen->BMask &= ~SCREEN_BMASK_STR_POLAR;

            dprif("BMask(strPolar) : %.4X\n", screen->BMask);
        }
        else if (!strcmp(key, "oePolar"))
        {
            vbool = json_object_get_boolean(val);
            dprif("oePolar : %d\n", vbool);
            if (vbool == FALSE)
                screen->BMask |= SCREEN_BMASK_OE_POLAR;
            else
                screen->BMask &= ~SCREEN_BMASK_OE_POLAR;

            dprif("BMask(oePolar) : %.4X\n", screen->BMask);
        }
        else if (!strcmp(key, "screenMirror"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                screen->BMask |= SCREEN_BMASK_MIRROR;
            else
                screen->BMask &= ~SCREEN_BMASK_MIRROR;

            dprif("BMask(screenMirror) : %.4X\n", screen->BMask);
        }
        else if (!strcmp(key, "screenSplit"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                screen->BMask |= SCREEN_BMASK_SPLIT;
            else
                screen->BMask &= ~SCREEN_BMASK_SPLIT;

            dprif("BMask(screenSplit) : %.4X\n", screen->BMask);
        }
        else if (!strcmp(key, "lineVerify"))
        {
            vint = json_object_get_int(val);
            if ((vint >= -16) && (vint <= 16))
            {
                screen->LineVerify = vint;
                dprif("LineVerify : %d\n", screen->LineVerify);
            }
            else
            {
                dprif("LineVerify error,current value %d\n", screen->LineVerify);
            }
        }
        else if (!strcmp(key, "lineBlank"))
        {
            vint = json_object_get_int(val);
            if ((vint <= 255) && (vint >= 0))
            {
                screen->LineBlank = vint;
                dprif("LineBlank : %d\n", screen->LineBlank);
            }
            else
            {
                dprif("LineBlank error,current value %d\n", screen->LineBlank);
            }
        }
        else if (!strcmp(key, "frequency"))
        {
            vint = json_object_get_int(val);
            if ((vint <= 30) && (vint >= 0))
            {
                screen->Frequency = vint;
                dprif("Frequency : %d\n", screen->Frequency);
            }
            else
            {
                dprif("Frequency error,current value %d\n", screen->Frequency);
            }
        }
        else if (!strcmp(key, "grayLevel"))
        {
            vint = json_object_get_int(val);
            if ((vint == 0) || (vint == 4) || (vint == 8)
              || (vint == 16) || (vint == 32) || (vint == 256))
            {
                if (vint == 0)
                    screen->BaseColor |= (uint8)(0 << 3);
                else if (vint == 4)
                    screen->BaseColor |= (uint8)(2 << 3);
                else if (vint == 8)
                    screen->BaseColor |= (uint8)(3 << 3);
                else if (vint == 16)
                    screen->BaseColor |= (uint8)(4 << 3);
                else if (vint == 32)
                    screen->BaseColor |= (uint8)(5 << 3);
                else if (vint == 256)
                    screen->BaseColor |= (uint8)(8 << 3);
                dprif("BaseColor(grayLevel) : %d\n", screen->BaseColor);
            }
            else
            {
                dprif("BaseColor(grayLevel) error,current value %d\n", screen->BaseColor);
            }
        }
        else if (!strcmp(key, "configIndex"))
        {
            screen->ConfigIndex = json_object_get_int(val);
            dprif("ConfigIndex : %d\n", screen->ConfigIndex);
        }
        else if (!strcmp(key, "extendInfo"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                screen->BMask |= SCREEN_BMASK_EXTEND_INFO;
            else
                screen->BMask &= ~SCREEN_BMASK_EXTEND_INFO;

            dprif("BMask(extendInfo) : %.4X\n", screen->BMask);
        }
        else
        {
            dprif("ScreenConfig key \"%s\" can not recognized\n", key);
        }
    }

    int i, gray = pow(2, GET_GRAY_LEVEL(screen->BaseColor));
    double tmp;
    dprif("GrayLevel : %d\n", gray);
    for(i = 0; i < 256; ++i)
    {
        tmp = pow(((double)i)/255, gama) * (double)(gray - 1);
        if (((int)(tmp*100) % 100) < 50)
            screen->Gama[i] = (int)tmp;
        else
            screen->Gama[i] = (int)(tmp + 1);
        dprif("Gama[%d] : %f, %d\n", i, tmp, screen->Gama[i]);
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
int Inspect_ScreenConfig(ScreenConfig_t *screen, Record_t *record)
{
    dprif("* Inspect_ScreenConfig\n");

    if (screen->Width == 0)
    {
        dprif("Width is in trouble\n");
        record->ScreenErrorno = RECORD_ERRORNO_ERROR;
    }

    if (screen->Height == 0)
    {
        dprif("Height is in trouble\n");
        record->ScreenErrorno = RECORD_ERRORNO_ERROR;
    }

    if (screen->ScanLine == 0)
    {
        dprif("ScanLine is in trouble\n");
        record->ScreenErrorno = RECORD_ERRORNO_ERROR;
    }

    if (screen->ScanMode == 0)
    {
        dprif("ScanMode is in trouble\n");
        record->ScreenErrorno = RECORD_ERRORNO_ERROR;
    }

    if (screen->BaseColor == 0)
    {
        dprif("BaseColor is in trouble\n");
        record->ScreenErrorno = RECORD_ERRORNO_ERROR;
    }

    if (screen->BaseColor == 0)
    {
        dprif("BaseColor is in trouble\n");
        record->ScreenErrorno = RECORD_ERRORNO_ERROR;
    }

    dprif("ScreenErrorno : %d\n", record->ScreenErrorno);

    return record->ScreenErrorno;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
extern const char ScreenConfigTable[][SCREEN_CONFIG_FILE_LENGTH];
int CreatFile_ScreenConfig(const char *path, ScreenConfig_t *screen, uint8 endian, Record_t *record)
{
    uint32 cnt;
    FILE *fp = NULL;
    FileHeader_t header;

    dprif("CreatFile_ScreenConfig\n");

    if ((path == NULL) || (screen == NULL))
    {
        dprif("CreatFile_ScreenConfig parameter error\n");
        goto out;
    }

    if (path[strlen(path) - 1] != '/')
    {
        dprif("CreatFile_ScreenConfig path error\n");
        goto out;
    }

    if (CreateDirectoryK(path) < 0)
        goto out;

    char file[PATH_LEN];

    if (endian == FILE_LITTLE_ENDIAN)
    {
        sprintf(file, "%s%s", path, "ALL.scf");
        memcpy(header.Sign, "SCF", 3);
    }
    else
    {
        sprintf(file, "%s%s", path, "ALL.scx");
        memcpy(header.Sign, "SCX", 3);
    }

    header.Version = FILE_VERSION;
    header.Length = sizeof(FileHeader_t) + (sizeof(ScreenConfig_t) - SCREEN_CONFIG_ADDED_LENGTH);
    header.Reserved = 0;

    if ((fp = fopen(file, "wb+")) == NULL)
    {
        dprif("fopen %s failed\n", file);
        goto out;
    }

    if ((screen->ScreenPath[0] != 0) && (screen->ScreenName[0] != 0))
    {
        dprif("CreatFile_ScreenConfig from ScreenPath and ScreenName\n");
        FILE *fpscreen = NULL;
        ScreenConfig_t tmpscreen;
        char content[SCREEN_CONFIG_FILE_LENGTH];
        char filepath[PATH_LEN + NAME_LEN];

        sprintf(filepath, "%s%s.SCF", screen->ScreenPath, screen->ScreenName);
        if ((fpscreen = fopen(filepath, "rb")) == NULL)
        {
            dprif("ScreenContent fopen %s failed\n", filepath);
            fclose(fp);
            goto out;
        }
        fread(content, sizeof(char), SCREEN_CONFIG_FILE_LENGTH, fpscreen);
        fclose(fpscreen);

        memcpy(&tmpscreen, &content[sizeof(FileHeader_t)], SCREEN_CONFIG_FILE_LENGTH);

        tmpscreen.Width = screen->Width;
        tmpscreen.Height = screen->Height;
        tmpscreen.BaseColor = screen->BaseColor;
        tmpscreen.Frequency = screen->Frequency;
        tmpscreen.LineBlank = screen->LineBlank;

#if 1
        tmpscreen.BMask &= ~SCREEN_BMASK_OE_POLAR;
        if (screen->BMask & SCREEN_BMASK_OE_POLAR)
            tmpscreen.BMask |= SCREEN_BMASK_OE_POLAR;

        tmpscreen.BMask &= ~SCREEN_BMASK_DATA_POLAR;
        if (screen->BMask & SCREEN_BMASK_DATA_POLAR)
            tmpscreen.BMask |= SCREEN_BMASK_DATA_POLAR;

        tmpscreen.BMask |= SCREEN_BMASK_STR_POLAR;
#endif
        screen->BMask &= ~SCREEN_BMASK_EXTEND_INFO;
        if (tmpscreen.BMask & SCREEN_BMASK_EXTEND_INFO)
            screen->BMask |= SCREEN_BMASK_EXTEND_INFO;

        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(content, sizeof(FileHeader_t), 1, fp);
        cnt = fwrite(&tmpscreen, sizeof(ScreenConfig_t) - SCREEN_CONFIG_ADDED_LENGTH, 1, fp);
    }
    else if (screen->ScreenContent[0] != 0)
    {
        dprif("CreatFile_ScreenConfig from ScreenContent\n");
        FILE *fpscreen = NULL;
        ScreenConfig_t tmpscreen;
        char content[SCREEN_CONFIG_FILE_LENGTH];

        if ((fpscreen = fopen(screen->ScreenContent, "rb")) == NULL)
        {
            dprif("ScreenContent fopen %s failed\n", screen->ScreenContent);
            fclose(fp);
            goto out;
        }
        fread(content, sizeof(char), SCREEN_CONFIG_FILE_LENGTH, fpscreen);
        fclose(fpscreen);

        memcpy(&tmpscreen, &content[sizeof(FileHeader_t)], SCREEN_CONFIG_FILE_LENGTH);

        tmpscreen.Width = screen->Width;
        tmpscreen.Height = screen->Height;
#if 0
        tmpscreen.BMask &= ~SCREEN_BMASK_OE_POLAR;
        if (screen->BMask & SCREEN_BMASK_OE_POLAR)
            tmpscreen.BMask |= SCREEN_BMASK_OE_POLAR;

        tmpscreen.BMask &= ~SCREEN_BMASK_DATA_POLAR;
        if (screen->BMask & SCREEN_BMASK_DATA_POLAR)
            tmpscreen.BMask |= SCREEN_BMASK_DATA_POLAR;
#endif
        screen->BMask &= ~SCREEN_BMASK_EXTEND_INFO;
        if (tmpscreen.BMask & SCREEN_BMASK_EXTEND_INFO)
            screen->BMask |= SCREEN_BMASK_EXTEND_INFO;
        screen->BaseColor = tmpscreen.BaseColor;

        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(content, sizeof(FileHeader_t), 1, fp);
        cnt = fwrite(&tmpscreen, sizeof(ScreenConfig_t) - SCREEN_CONFIG_ADDED_LENGTH, 1, fp);
    }
    else if (screen->ConfigIndex == 0)
    {
        dprif("CreatFile_ScreenConfig from parameter\n");
        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

        fseek(fp, sizeof(FileHeader_t), SEEK_SET);
        cnt = fwrite(screen, (sizeof(ScreenConfig_t) - SCREEN_CONFIG_ADDED_LENGTH), 1, fp);
    }
    else if (screen->ConfigIndex < SCREEN_FILE_MAX)
    {
        dprif("CreatFile_ScreenConfig from ConfigIndex\n");
        ScreenConfig_t tmpscreen;
        memcpy(&tmpscreen, &ScreenConfigTable[screen->ConfigIndex][sizeof(FileHeader_t)], SCREEN_CONFIG_FILE_LENGTH);
        tmpscreen.Width = screen->Width;
        tmpscreen.Height = screen->Height;

        tmpscreen.BMask &= ~SCREEN_BMASK_OE_POLAR;
        if (screen->BMask & SCREEN_BMASK_OE_POLAR)
            tmpscreen.BMask |= SCREEN_BMASK_OE_POLAR;

        tmpscreen.BMask &= ~SCREEN_BMASK_DATA_POLAR;
        if (screen->BMask & SCREEN_BMASK_DATA_POLAR)
            tmpscreen.BMask |= SCREEN_BMASK_DATA_POLAR;

        screen->BMask &= ~SCREEN_BMASK_EXTEND_INFO;
        if (tmpscreen.BMask & SCREEN_BMASK_EXTEND_INFO)
            screen->BMask |= SCREEN_BMASK_EXTEND_INFO;
        screen->BaseColor = tmpscreen.BaseColor;

        fseek(fp, 0, SEEK_SET);
        cnt = fwrite(ScreenConfigTable[screen->ConfigIndex], sizeof(FileHeader_t), 1, fp);
        cnt = fwrite(&tmpscreen, sizeof(ScreenConfig_t), 1, fp);
    }
    else
    {
        fclose(fp);
        goto out;
    }

    fclose(fp);

    dprif("CreatFile_ScreenConfig OK!\n");

    return 0;

out:
    dprif("CreatFile_ScreenConfig fail!\n");
    record->ScreenErrorno = RECORD_ERRORNO_CREATE_FAIL;
    return -1;
}

