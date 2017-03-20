
#include "kaler_config.h"
#include "../json/json.h"
#include "parse_display.h"

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
int JsonParse_DateTime(const json_object *new_obj, DateTime_t *dtime, int mode)
{
    char vstring[JSON_STRING_LEN];
    char *ptmp;

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "date"))
        {
            strcpy(vstring, json_object_get_string(val));
            ptmp = strtok(vstring, "-");//
            if (ptmp != NULL) dtime->Year = (uint16)atoi((char*)ptmp);
            ptmp = strtok(NULL, "-");//
            if (ptmp != NULL) dtime->Month = (uint16)atoi((char*)ptmp);
            ptmp =strtok(NULL, "\0");//
            if (ptmp != NULL) dtime->Day = (uint16)atoi((char*)ptmp);
        }
        else if (!strcmp(key, "week"))
        {
            if (mode == PARSE_WEEK_MODE_NUMBER)
                dtime->Week = json_object_get_int(val);
            else if (mode == PARSE_WEEK_MODE_MASK)
            {
                int i;
                strcpy(vstring, json_object_get_string(val));

                dtime->Week = 0;
                for (i = 0; i < 7; i++)
                    if (vstring[i] == '1')
                        dtime->Week |= (1 << i);
                if(dtime->Week != 0xEF)
                {
                    dtime->Week |= (1 << 7);
                }
            }
            else
            {
                dprif("week mode error\n");
            }
        }
        else if (!strcmp(key, "time"))
        {
            strcpy(vstring, json_object_get_string(val));
            ptmp = strtok(vstring, ":");//
            if (ptmp != NULL) dtime->Hour = (uint16)atoi((char*)ptmp);
            ptmp = strtok(NULL, ":");//
            if (ptmp != NULL) dtime->Min = (uint16)atoi((char*)ptmp);
            ptmp =strtok(NULL, "\0");//
            if (ptmp != NULL) dtime->Sec = (uint16)atoi((char*)ptmp);
        }
        else
        {
            dprif("key \"%s\" can not recognized\n", key);
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
int JsonParse_DisplayConfig(const json_object *new_obj, DisplayConfig_t *display, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;

    memset(display, 0, sizeof(DisplayConfig_t));

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "bright"))
        {
            vint = json_object_get_int(val);
            if ((vint < MAX_BRIGHT) && (vint >= 0))
            {
                display->Bright = vint;
                dprif("display->Bright : %d\n", display->Bright);
            }
            else
            {
                display->Bright = MAX_BRIGHT;
                dprif("display->Bright error,current value %d\n", display->Bright);
            }
        }
        else if (!strcmp(key, "enAutoBright"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_AUTO_BRIGHT;
            else
                display->BMask &= ~DISPLAY_BMASK_AUTO_BRIGHT;

            dprif("display->BMask(enAutoBright) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "updateBright"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_UPDATE_BRIGHT;
            else
                display->BMask &= ~DISPLAY_BMASK_UPDATE_BRIGHT;

            dprif("display->BMask(updateBright) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "updateTime"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_UPDATE_TIME;
            else
                display->BMask &= ~DISPLAY_BMASK_UPDATE_TIME;

            dprif("display->BMask(updateTime) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "enRelativeTime"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_RELATIVE_TIME;
            else
                display->BMask &= ~DISPLAY_BMASK_RELATIVE_TIME;
            dprif("display->BMask(enRelativeTime) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "enAutoOpenScreen"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_AUTO_OPEN_SCREEN;
            else
                display->BMask &= ~DISPLAY_BMASK_AUTO_OPEN_SCREEN;

            dprif("display->BMask(enAutoOpenScreen) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "enAutoCloseScreen"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_AUTO_CLOSE_SCREEN;
            else
                display->BMask &= ~DISPLAY_BMASK_AUTO_CLOSE_SCREEN;

            dprif("display->BMask(enAutoCloseScreen) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "updateTimingBright"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                display->BMask |= DISPLAY_BMASK_UPDATE_TIMEING_BRIGHT;
            else
                display->BMask &= ~DISPLAY_BMASK_UPDATE_TIMEING_BRIGHT;

            dprif("display->BMask(updateTimingBright) : %.4X\n", display->BMask);
        }
        else if (!strcmp(key, "currentTime"))
        {
            JsonParse_DateTime(val, &display->CurrentTime, PARSE_WEEK_MODE_NUMBER);
            dprif("currentTime date : %d-%d-%d\n", display->CurrentTime.Year, display->CurrentTime.Month, display->CurrentTime.Day);
            dprif("currentTime week : %d\n", display->CurrentTime.Week);
            dprif("currentTime time : %d:%d:%d\n", display->CurrentTime.Hour, display->CurrentTime.Min, display->CurrentTime.Sec);
        }
        else if (!strcmp(key, "closeScreenTime"))
        {
            json_object_object_foreach((json_object *)val, key_1, val_1)
            {
                if (!strcmp(key_1, "time"))
                {
                    JsonParse_DateTime(val_1, &display->CloseScreenTime, PARSE_WEEK_MODE_NUMBER);
                    dprif("CloseScreenTime date : %d-%d-%d\n", display->CloseScreenTime.Year, display->CloseScreenTime.Month, display->CloseScreenTime.Day);
                    dprif("CloseScreenTime week : %d\n", display->CloseScreenTime.Week);
                    dprif("CloseScreenTime time : %d:%d:%d\n", display->CloseScreenTime.Hour, display->CloseScreenTime.Min, display->CloseScreenTime.Sec);
                }
                else if (!strcmp(key_1, "cmpYear"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_YEAR;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_YEAR;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpMonth"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_MONTH;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_MONTH;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpDay"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_DAY;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_DAY;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpHour"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_HOUR;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_HOUR;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpMin"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_MIN;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_MIN;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpSec"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_SEC;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_SEC;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpSUN"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_SUN;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_SUN;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpMON"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_MON;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_MON;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpTUE"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_TUE;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_TUE;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpWED"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_WED;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_WED;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpTHU"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_THU;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_THU;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpFRI"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_FRI;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_FRI;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
                else if (!strcmp(key_1, "cmpSAT"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->CloseScreenMask |= DISPLAY_TIMECMP_SAT;
                    else
                        display->CloseScreenMask &= ~DISPLAY_TIMECMP_SAT;

                    dprif("display->CloseScreenMask : %.4X\n", display->CloseScreenMask);
                }
            }
        }
        else if (!strcmp(key, "openScreenTime"))
        {
            json_object_object_foreach((json_object *)val, key_1, val_1)
            {
                if (!strcmp(key_1, "time"))
                {
                    JsonParse_DateTime(val_1, &display->OpenScreenTime, PARSE_WEEK_MODE_NUMBER);
                    dprif("OpenScreenTime date : %d-%d-%d\n", display->OpenScreenTime.Year, display->OpenScreenTime.Month, display->OpenScreenTime.Day);
                    dprif("OpenScreenTime week : %d\n", display->OpenScreenTime.Week);
                    dprif("OpenScreenTime time : %d:%d:%d\n", display->OpenScreenTime.Hour, display->OpenScreenTime.Min, display->OpenScreenTime.Sec);
                }
                else if (!strcmp(key_1, "cmpYear"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_YEAR;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_YEAR;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpMonth"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_MONTH;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_MONTH;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpDay"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_DAY;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_DAY;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpHour"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_HOUR;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_HOUR;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpMin"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_MIN;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_MIN;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpSec"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_SEC;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_SEC;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpSUN"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_SUN;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_SUN;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpMON"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_MON;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_MON;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpTUE"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_TUE;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_TUE;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpWED"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_WED;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_WED;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpTHU"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_THU;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_THU;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpFRI"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_FRI;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_FRI;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
                else if (!strcmp(key_1, "cmpSAT"))
                {
                    vbool = json_object_get_boolean(val_1);
                    if (vbool == TRUE)
                        display->OpenScreenMask |= DISPLAY_TIMECMP_SAT;
                    else
                        display->OpenScreenMask &= ~DISPLAY_TIMECMP_SAT;

                    dprif("display->OpenScreenMask : %.4X\n", display->OpenScreenMask);
                }
            }
        }
        else if (!strcmp(key, "timingBright"))
        {
            json_object_object_foreach((json_object *)val, key_1, val_1)
            {
                if (!strcmp(key_1, "defaultBright"))
                {
                    vint = json_object_get_int(val_1);
                    if ((vint < MAX_BRIGHT) && (vint >= 0))
                    {
                        display->TimingBright.DefaultBright = vint;
                        dprif("display->TimingBright.DefaultBright : %d\n", display->TimingBright.DefaultBright);
                    }
                    else
                    {
                        display->TimingBright.DefaultBright = MAX_BRIGHT;
                        dprif("display->Bright error,current value %d\n", display->Bright);
                    }
                }
                else if (!strcmp(key_1, "tBrightArray"))
                {
                    int i;
                    char *ptmp;
                    int arraylen = json_object_array_length(val_1);

                    if ((arraylen > DISPLAY_TBRIGHT_LENGTH) || (arraylen < 0))
                    {
                        arraylen = 0;
                        dprif("tBrightArray number error set to 0");
                    }
                    display->TimingBright.TimeNum = arraylen;
                    display->TimingBright.TMask = 0xff >> (8 - arraylen);
                    dprif("TimingBright number:%d, mask:%.X\n",
                        display->TimingBright.TimeNum,
                        display->TimingBright.TMask);

                    for (i = 0; i < arraylen; i++)
                    {
                        TBright_t *tbptr = &display->TimingBright.TBright[i];

                        json_object_object_foreach(json_object_array_get_idx(val_1, i), key_2, val_2)
                        {
                            if (!strcmp(key_2, "beginTime"))
                            {
                                strcpy(vstring, json_object_get_string(val_2));
                                ptmp = strtok(vstring, ":");//
                                if (ptmp != NULL) tbptr->BeginTime.Hour = (uint16)atoi((char*)ptmp);
                                ptmp = strtok(NULL, ":");//
                                if (ptmp != NULL) tbptr->BeginTime.Min = (uint16)atoi((char*)ptmp);
                                ptmp =strtok(NULL, "\0");//
                                if (ptmp != NULL) tbptr->BeginTime.Sec = (uint16)atoi((char*)ptmp);
                                dprif("TimingBright[%d] begin time : %d:%d:%d\n", i,
                                    tbptr->BeginTime.Hour,
                                    tbptr->BeginTime.Min,
                                    tbptr->BeginTime.Sec);

                            }
                            else if (!strcmp(key_2, "endTime"))
                            {
                                strcpy(vstring, json_object_get_string(val_2));
                                ptmp = strtok(vstring, ":");//
                                if (ptmp != NULL) tbptr->EndTime.Hour = (uint16)atoi((char*)ptmp);
                                ptmp = strtok(NULL, ":");//
                                if (ptmp != NULL) tbptr->EndTime.Min = (uint16)atoi((char*)ptmp);
                                ptmp =strtok(NULL, "\0");//
                                if (ptmp != NULL) tbptr->EndTime.Sec = (uint16)atoi((char*)ptmp);
                                dprif("TimingBright[%d] end time : %d:%d:%d\n", i,
                                    tbptr->EndTime.Hour,
                                    tbptr->EndTime.Min,
                                    tbptr->EndTime.Sec);
                            }
                            else if (!strcmp(key_2, "tBright"))
                            {
                                vint = json_object_get_int(val_2);
                                if ((vint < MAX_BRIGHT) && (vint >= 0))
                                {
                                    tbptr->Bright = vint;
                                    dprif("TimingBright[%d].Bright : %d\n", i, tbptr->Bright);
                                }
                                else
                                {
                                    tbptr->Bright = MAX_BRIGHT;
                                    dprif("TimingBright[%d].Bright error,current value %d\n", i, tbptr->Bright);
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            dprif("DisplayConfig key \"%s\" can not recognized\n", key);
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
int Inspect_DisplayConfig(DisplayConfig_t *display, Record_t *record)
{
    dprif("* Inspect_DisplayConfig\n");

    dprif("DisplayErrorno : %d\n", record->DisplayErrorno);

    return record->DisplayErrorno;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CreatFile_DisplayConfig(const char * path, const DisplayConfig_t *display, uint8 endian, Record_t *record)
{
    uint32 cnt;
    FILE *fp = NULL;
    FileHeader_t header;

    dprif("CreatFile_DisplayConfig\n");

    if ((path == NULL) || (display == NULL))
    {
        dprif("CreatFile_DisplayConfig parameter error\n");
        goto out;
    }

    if (path[strlen(path) - 1] != '/')
    {
        dprif("CreatFile_DisplayConfig path error\n");
        goto out;
    }

    if (CreateDirectoryK(path) < 0)
        goto out;

    char file[PATH_LEN];

    if (endian == FILE_LITTLE_ENDIAN)
    {
        sprintf(file, "%s%s", path, "ALL.dcf");
        memcpy(header.Sign, "DCF", 3);
    }
    else
    {
        sprintf(file, "%s%s", path, "ALL.dcx");
        memcpy(header.Sign, "DCX", 3);
    }

    header.Version = FILE_VERSION;
    header.Length = sizeof(FileHeader_t) + sizeof(DisplayConfig_t);
    header.Reserved = 0;

    if ((fp = fopen(file, "wb+")) == NULL)
    {
        dprif("fopen %s failed\n", file);
        goto out;
    }

    fseek(fp, 0, SEEK_SET);
    cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

    fseek(fp, sizeof(FileHeader_t), SEEK_SET);
    cnt = fwrite(display, sizeof(DisplayConfig_t), 1, fp);

    fclose(fp);

    dprif("CreatFile_DisplayConfig OK!\n");

    return 0;

out:
    record->DisplayErrorno = RECORD_ERRORNO_CREATE_FAIL;
    return -1;
}

