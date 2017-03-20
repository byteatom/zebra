
#include "kaler_config.h"
#include "../json/json.h"
#include "parse_top.h"
#include "parse_display.h"
#include "parse_screen.h"
#include "parse_scandot.h"
#include "parse_network.h"
#include "parse_program.h"

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
int JsonParse_FirstGetTopLevelConfig(const json_object *new_obj, TopLevelConfig_t *toplevel, Record_t *record);

int JsonParseAll(const char *json_buf, AllConfig_t *allptr, Record_t *record)
{
    json_object *new_obj;

    if ((new_obj = json_tokener_parse(json_buf)) == NULL)
    {
        dprif("json_tokener_parse fail\n");
        return -1;
    }

    // must get top level config before read other json key
    if (JsonParse_FirstGetTopLevelConfig(new_obj, &allptr->TopLevelConfig, record))
    {
        dprif("TopLevel critical error\n");
        return -2;
    }

    json_object_object_foreach(new_obj, key, val)
    {
        if (!strcmp(key, JSONOB_LED_SCREEN_CONFIG))
        {
            dprif("**************************************************\n");
            dprif(JSONOB_LED_SCREEN_CONFIG"\n");
            record->ScreenExist = RECORD_FILE_EXIST_YES;
            JsonParse_ScreenConfig(val, &allptr->ScreenConfig, record);
            Inspect_ScreenConfig(&allptr->ScreenConfig, record);
        }
        else if (!strcmp(key, JSONOB_SCAN_DOT_CONFIG))
        {
            dprif("**************************************************\n");
            dprif(JSONOB_SCAN_DOT_CONFIG"\n");
            record->ScanDotExist = RECORD_FILE_EXIST_YES;
            JsonParse_ScanDotConfig(val, &allptr->ScanDotConfig, record);
            Inspect_ScanDotConfig(&allptr->ScanDotConfig, record);
        }
        else if (!strcmp(key, JSONOB_DISPLAY_CONFIG))
        {
            dprif("**************************************************\n");
            dprif(JSONOB_DISPLAY_CONFIG"\n");
            record->DisplayExist = RECORD_FILE_EXIST_YES;
            JsonParse_DisplayConfig(val, &allptr->DisplayConfig, record);
            Inspect_DisplayConfig(&allptr->DisplayConfig, record);
        }
        else if (!strcmp(key, JSONOB_NETWORK_CONFIG))
        {
            dprif("**************************************************\n");
            dprif(JSONOB_NETWORK_CONFIG"\n");
            record->NetworkExist = RECORD_FILE_EXIST_YES;
            JsonParse_NetworkConfig(val, &allptr->NetworkConfig, record);
            Inspect_NetworkConfig(&allptr->NetworkConfig, record);
        }
        else if (!strcmp(key, JSONOB_PROGRAM_DESCRIBE))
        {
            dprif("**************************************************\n");
            dprif(JSONOB_PROGRAM_DESCRIBE"\n");
            record->ProgramExist = RECORD_FILE_EXIST_YES;
            JsonParse_ProgramDescribe(val, &allptr->ProgramDescribe, record);
            Inspect_ProgramDescribe(&allptr->ProgramDescribe, record);
        }
    }

    json_object_put(new_obj);

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CreateKalerFile(const char *outpath, AllConfig_t *allptr, Record_t *record)
{
    dprif("**************************************************\n");
    dprif("CreateKalerFile\n");

    if ((record->ScreenExist == RECORD_FILE_EXIST_YES)
      && (record->ScreenErrorno == RECORD_ERRORNO_OK))
    {
        CreatFile_ScreenConfig(outpath,
                            &allptr->ScreenConfig,
                            allptr->TopLevelConfig.Endian,
                            record);

        if (((record->ScanDotExist == RECORD_FILE_EXIST_YES) && (record->ScanDotErrorno == RECORD_ERRORNO_OK))
          || ((allptr->ScreenConfig.BMask & SCREEN_BMASK_EXTEND_INFO) &&
            ((allptr->ScreenConfig.ConfigIndex != 0) || (allptr->ScreenConfig.ScanDotContent[0] != 0) || ((allptr->ScreenConfig.ScreenPath[0] != 0) && (allptr->ScreenConfig.ScreenName[0] != 0)))))
        {
            CreatFile_ScanDotConfig(outpath,
                                &allptr->ScanDotConfig,
                                &allptr->ScreenConfig,
                                allptr->TopLevelConfig.Endian,
                                record);
        }
    }

    if ((record->DisplayExist == RECORD_FILE_EXIST_YES)
      && (record->DisplayErrorno == RECORD_ERRORNO_OK))
    {
        CreatFile_DisplayConfig(outpath,
                            &allptr->DisplayConfig,
                            allptr->TopLevelConfig.Endian,
                            record);
    }

    if ((record->NetworkExist == RECORD_FILE_EXIST_YES)
      && (record->NetworkErrorno == RECORD_ERRORNO_OK))
    {
        CreatFile_NetworkConfig(outpath,
                            &allptr->NetworkConfig,
                            allptr->TopLevelConfig.Endian,
                            record);
    }

    if ((record->ProgramExist == RECORD_FILE_EXIST_YES)
      && (record->NetworkErrorno == RECORD_ERRORNO_OK))
    {
        int i, j;
        PictureKarma_t karma;
        karma.BaseColor = allptr->ScreenConfig.BaseColor;
        karma.Endian = allptr->TopLevelConfig.Endian;
        karma.ColorThreshold = allptr->TopLevelConfig.ColorThreshold;
        karma.Gama = allptr->ScreenConfig.Gama;
        karma.DataPolar = (allptr->ScreenConfig.BMask & SCREEN_BMASK_DATA_POLAR) ? 1 : 0;

        CreatFile_ProgramCCF(outpath,
                            &allptr->ProgramDescribe.CCF,
                            allptr->TopLevelConfig.Endian,
                            record);

        for (i = 0; i < allptr->ProgramDescribe.CCF.ProgramTotal; i++)
        {
            CreatFile_ProgramBCF(outpath,
                                &allptr->ProgramDescribe.BCF[i],
                                &karma,
                                record);

            CreatFile_ProgramPCF(outpath,
                                &allptr->ProgramDescribe.PCF[i],
                                &karma,
                                record);

            for (j = 0; j < allptr->ProgramDescribe.PCF[i].AreaTotal; j++)
            {
                switch (allptr->ProgramDescribe.PCF[i].Area[j].Type)
                {
                case AREA_WORD: {
                        CreatFile_ACFWord(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                case AREA_TIME: {
                        CreatFile_ACFTime(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                case AREA_SHINE: {
                        CreatFile_ACFShine(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                case AREA_WATCH: {
                        CreatFile_ACFWatch(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                case AREA_COUNT: {
                        CreatFile_ACFCount(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                case AREA_SENSOR: {
                        CreatFile_ACFTemperature(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                case AREA_COLORFUL: {
                        CreatFile_ACFColorful(outpath,
                                        &allptr->ProgramDescribe.PCF[i].Area[j],
                                        &karma,
                                        record);
                        break;
                    }
                default:
                    break;
                }
            }
        }

        CreatFile_ProgramConcord(outpath,
                                &allptr->ProgramDescribe,
                                allptr->TopLevelConfig.Endian,
                                record);
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
int FreeMemoryAll(AllConfig_t *allptr, Record_t *record)
{
    int i, j;

    for (i = 0; i < allptr->ProgramDescribe.CCF.ProgramTotal; i++)
    {
        for (j = 0; j < allptr->ProgramDescribe.PCF[i].AreaTotal; j++)
        {
            switch (allptr->ProgramDescribe.PCF[i].Area[j].Type)
            {
            case AREA_WORD: {
                    FreeMemory_ACFWord(&allptr->ProgramDescribe.PCF[i].Area[j], record);
                    break;
                }
            case AREA_TIME: {
                    FreeMemory_ACFTime(&allptr->ProgramDescribe.PCF[i].Area[j], record);
                    break;
                }
            case AREA_SHINE: {
                    FreeMemory_ACFShine(&allptr->ProgramDescribe.PCF[i].Area[j], record);
                    break;
                }
            case AREA_WATCH: {
                    FreeMemory_ACFWatch(&allptr->ProgramDescribe.PCF[i].Area[j], record);
                    break;
                }
            case AREA_COUNT: {
                    FreeMemory_ACFCount(&allptr->ProgramDescribe.PCF[i].Area[j], record);
                    break;
                }
            case AREA_SENSOR: {
                    FreeMemory_ACFTemperature(&allptr->ProgramDescribe.PCF[i].Area[j], record);
                    break;
                }
            default:
                break;
            }
        }
    }

    FreeMemory_ProgramPCF(&allptr->ProgramDescribe, record);
    FreeMemory_ProgramBCF(&allptr->ProgramDescribe, record);
    FreeMemory_ProgramCCF(&allptr->ProgramDescribe, record);

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_FirstGetTopLevelConfig(const json_object *new_obj, TopLevelConfig_t *toplevel, Record_t *record)
{
    int ret = 0;

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, JSONOB_TOP_LEVEL_CONFIG))
        {
            dprif(JSONOB_TOP_LEVEL_CONFIG"\n");
            record->TopLevelExist = RECORD_FILE_EXIST_YES;
            JsonParse_TopLevelConfig(val, toplevel, record);
            ret = Inspect_TopLevelConfig(toplevel, record);
        }
    }

    return ret;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_TopLevelConfig(const json_object *new_obj, TopLevelConfig_t *toplevel, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    int endian_confirm = NO;

    memset(toplevel, 0, sizeof(TopLevelConfig_t));

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        // dataEndianprivateModeldataEndian
        if (!strcmp(key, "dataEndian"))
        {
            strcpy(vstring, json_object_get_string(val));

            if(!strcmp(vstring, "all"))
                toplevel->Endian = FILE_ALL_ENDIAN;
            else if(!strcmp(vstring, "big"))
                toplevel->Endian = FILE_BIG_ENDIAN;
            else if(!strcmp(vstring, "little"))
                toplevel->Endian = FILE_LITTLE_ENDIAN;
            else
            {
                dprif("dataEndian key error, set dataEndian to all\n");
                toplevel->Endian = FILE_ALL_ENDIAN;
            }

            endian_confirm = YES;

            dprif("Endian : %d\n", toplevel->Endian);
        }
        else if (!strcmp(key, "privateModel"))
        {
            if (endian_confirm == NO)
            {
                strcpy(vstring, json_object_get_string(val));

                if(!strcmp(vstring, "all"))
                {
                    toplevel->Endian = FILE_ALL_ENDIAN;
                    strcpy(toplevel->CardModel, vstring);
                }
                else if(!strcmp(vstring, "x4s"))
                {
                    toplevel->Endian = FILE_BIG_ENDIAN;
                    strcpy(toplevel->CardModel, vstring);
                }
                else if(!strcmp(vstring, "x8"))
                {
                    toplevel->Endian = FILE_LITTLE_ENDIAN;
                    strcpy(toplevel->CardModel, vstring);
                }
                else
                {
                    dprif("PrivateModel key error, set privateModel to all\n");
                    toplevel->Endian = FILE_ALL_ENDIAN;
                    strcpy(toplevel->CardModel, "all");
                }

                dprif("PrivateModel : %d\n", toplevel->Endian);
            }
            else
            {
                dprif("DataEndian has been confirm to %d\n", toplevel->Endian);
            }
        }
        else if (!strcmp(key, "outFilePath"))
        {
            strcpy(vstring, json_object_get_string(val));
            if (strlen(vstring) < PATH_LEN)
            {
                strcpy(toplevel->OutFilePath, vstring);
                if (toplevel->OutFilePath[strlen(toplevel->OutFilePath) - 1] != '/')
                {
                    toplevel->OutFilePath[strlen(toplevel->OutFilePath) + 0] = '/';
                    toplevel->OutFilePath[strlen(toplevel->OutFilePath) + 1] = 0;
                }
                dprif("OutFilePath : %s\n", toplevel->OutFilePath);
            }
            else
            {
                dprif("OutFilePath critical error\n");
            }
        }
        else if (!strcmp(key, "colorThreshold"))
        {
            toplevel->ColorThreshold = json_object_get_int(val);
            dprif("ColorThreshold : %d\n", toplevel->ColorThreshold);
        }
        else
        {
            dprif("TopConfig key \"%s\" can not recognized\n", key);
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
int Inspect_TopLevelConfig(TopLevelConfig_t *toplevel, Record_t *record)
{
    dprif("* Inspect_TopLevelConfig\n");

    if (toplevel->OutFilePath[0] == 0)
    {
        dprif("OutFilePath is in trouble\n");
        record->TopLevelErrorno = RECORD_ERRORNO_CRITICAL;
    }

    dprif("TopLevelErrorno : %d\n", record->TopLevelErrorno);

    return record->TopLevelErrorno;
}
