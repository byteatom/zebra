
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kaler_config.h"
#include "kaler_struct.h"
#include "file_ops.h"
#include "parse_top.h"
#include "parse_protocol.h"

#ifdef undprif
#undef dprif
#define dprif(fmt, ...)
#endif


/*
 *******************************************************************************
 * author:
 * date:
 * description:Json
 *******************************************************************************
 */
int JsonParseAndCreateFile(const char *jsonpath)
{
    int ret = 0;
    int errorno = 0;
    char *jsonbuf;
    AllConfig_t AllConfig;
    Record_t Record;

    memset(&Record, 0, sizeof(Record_t));

    if ((ret = InitJsonFileBuf(jsonpath, &jsonbuf)) != 0)
    {
        dprif("initJsonFileBuf fild : %d", ret);
        errorno = -1;
        goto out1;
    }

    if ((ret = JsonParseAll(jsonbuf, &AllConfig, &Record)) != 0)
    {
        dprif("JsonParse_TopLevel fild : %d", ret);
        errorno = -2;
        goto out2;
    }

    dprif("TopLevelConfig.OutFilePath %s\n",AllConfig.TopLevelConfig.OutFilePath);

    if (AllConfig.TopLevelConfig.Endian == FILE_ALL_ENDIAN)
    {
        AllConfig.TopLevelConfig.Endian = FILE_BIG_ENDIAN;
        CreateKalerFile(AllConfig.TopLevelConfig.OutFilePath, &AllConfig, &Record);
        AllConfig.TopLevelConfig.Endian = FILE_LITTLE_ENDIAN;
        CreateKalerFile(AllConfig.TopLevelConfig.OutFilePath, &AllConfig, &Record);
    }
    else
    {
        CreateKalerFile(AllConfig.TopLevelConfig.OutFilePath, &AllConfig, &Record);
    }

    FreeMemoryAll(&AllConfig, &Record);

out2:
    FreeJsonFileBuf(jsonbuf);
out1:
    return errorno;
}


/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int SplitBMPFileHorizontal(const char *bmppath, const char *dir, const char *startname, const Coord_t *coord, uint32 *splitnum)
{
    FILE *fp;
    int32 i,j;
    BMPFileHeader_t bmpfileheader;
    BMPInfoHeader_t bmpinfoheader;
    uint16 bmpfilenum;
    uint32 bmpfilelen;
    uint32 bmpposition = sizeof(BMPInfoHeader_t) + sizeof(BMPFileHeader_t);
    char **filecontent;

    *splitnum = 0;

    if (coord->Width <= 0 || coord->Height <= 0)
    {
        return GETPICTURE_ERROR;
    }

    if ((fp = fopen(bmppath, "rb")) == NULL)
    {
        return GETPICTURE_NOFILE;
    }

    fread(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
    fread(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

    if((0 !=memcmp((char*)&bmpfileheader.Type, "BM", 2))
      &&(0 !=memcmp((char*)&bmpfileheader.Type, "MB", 2)))
    {
        fclose(fp);
        return GETPICTURE_NOTBMP;
    }

    CreateDirectoryK(dir);

    bmpfilenum = (bmpinfoheader.Width +coord->Width -1)/coord->Width;
    *splitnum = bmpfilenum;

    if(bmpinfoheader.BitCount == 32)
    {
        //dprif("bit 32\n");
        uint16 srclinelenbyte = 4*bmpinfoheader.Width;
        uint16 dstlinelenbyte = 4*coord->Width;

        bmpfilelen = bmpposition + (dstlinelenbyte * coord->Height);
        filecontent = malloc(sizeof(char*) * bmpfilenum);
        for (i = 0; i < bmpfilenum; i++)
            filecontent[i] = malloc(bmpfilelen);

        if (bmpinfoheader.Height > coord->Height)
            fseek(fp, bmpposition + (srclinelenbyte * (bmpinfoheader.Height - coord->Height)) ,SEEK_SET);

        for (i = 0; i < coord->Height; i++)
        {
            for (j = 0; j < bmpinfoheader.Width/coord->Width; ++j)
                fread(filecontent[j] + bmpposition + (i*dstlinelenbyte), 1, dstlinelenbyte, fp);

            if (bmpinfoheader.Width%coord->Width != 0)
            {
                fread(filecontent[j] + bmpposition + (i*dstlinelenbyte), 1, 4*(bmpinfoheader.Width%coord->Width), fp);
                memset(filecontent[j] + bmpposition + (i*dstlinelenbyte) + 4*(bmpinfoheader.Width%coord->Width),
                        0, 4*(coord->Width - (bmpinfoheader.Width%coord->Width)));
            }
        }
    }
    else if(bmpinfoheader.BitCount == 24)
    {
        //dprif("bit 24\n");
        uint8 srclineremain = ((3*bmpinfoheader.Width)%4 ? 4 - (3*bmpinfoheader.Width)%4 : 0);
        uint8 dstlineremain = ((3*coord->Width)%4 ? 4 - (3*coord->Width)%4 : 0);
        uint16 srclinelenbyte = (3*bmpinfoheader.Width) + srclineremain;
        uint16 dstlinelenbyte = (3*coord->Width) + dstlineremain;

        bmpfilelen = bmpposition + (dstlinelenbyte * coord->Height);
        filecontent = malloc(sizeof(char*) * bmpfilenum);
        for (i = 0; i < bmpfilenum; i++)
            filecontent[i] = malloc(bmpfilelen);

        if (bmpinfoheader.Height > coord->Height)
            fseek(fp, bmpposition + (srclinelenbyte * (bmpinfoheader.Height - coord->Height)) ,SEEK_SET);

        for (i = 0; i < coord->Height; ++i)
        {
            for (j = 0; j < bmpinfoheader.Width/coord->Width; ++j)
                fread(filecontent[j] + bmpposition + (i*dstlinelenbyte), 1, 3*coord->Width, fp);

            if (bmpinfoheader.Width%coord->Width != 0)
            {
                fread(filecontent[j] + bmpposition + (i*dstlinelenbyte), 1, 3*(bmpinfoheader.Width%coord->Width), fp);
                fseek(fp, srclineremain, SEEK_CUR);
                memset(filecontent[j] + bmpposition + (i*dstlinelenbyte) + 3*(bmpinfoheader.Width%coord->Width),
                        0, 3*(coord->Width - (bmpinfoheader.Width%coord->Width)));
            }
        }
    }
    else
    {
        fclose(fp);
        return GETPICTURE_NOT24OR32BITS;
    }

    FILE *cfp;
    char path[PATH_LEN];
    bmpinfoheader.Width =coord->Width;
    bmpinfoheader.Height =coord->Height;
    for (i = 0; i < bmpfilenum; i++)
    {
        memcpy(filecontent[i], &bmpfileheader, sizeof(BMPFileHeader_t));
        memcpy(filecontent[i] + sizeof(BMPFileHeader_t), &bmpinfoheader, sizeof(BMPInfoHeader_t));

        if (dir[strlen(dir) - 1] != '/')
            sprintf(path, "%s/%s_%d.bmp", dir, startname, i);
        else
            sprintf(path, "%s%s_%d.bmp", dir, startname, i);

        dprif("CreatSplitBMPFile:%s\n", path);

        if ((cfp = fopen(path, "wb")) == NULL)
        {
            for (i = 0; i < bmpfilenum; i++)
            {
                if (filecontent[i] != NULL)
                    free(filecontent[i]);
            }
            if (filecontent[i] != NULL)
                free(filecontent);

            fclose(fp);
            return GETPICTURE_NOFILE;
        }

        fwrite(filecontent[i], sizeof(char), bmpfilelen, cfp);
        fclose(cfp);
    }

    for (i = 0; i < bmpfilenum; i++)
    {
        if (filecontent[i] != NULL)
            free(filecontent[i]);
    }
    if (filecontent[i] != NULL)
        free(filecontent);

    fclose(fp);

    return GETPICTURE_SUCCESS;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int SplitBMPFileVerticle(const char *bmppath, const char *dir, const char *startname, const Coord_t *coord, uint32 *splitnum)
{
    FILE *fp;
    int32 i,j;
    BMPFileHeader_t bmpfileheader;
    BMPInfoHeader_t bmpinfoheader;
    uint16 bmpfilenum;
    uint32 bmpfilelen;
    uint32 bmpposition = sizeof(BMPInfoHeader_t) + sizeof(BMPFileHeader_t);
    char **filecontent;

    *splitnum = 0;

    if((fp = fopen(bmppath, "rb")) == NULL)
    {
        return GETPICTURE_NOFILE;
    }

    fread(&bmpfileheader, sizeof(BMPFileHeader_t), 1, fp);
    fread(&bmpinfoheader, sizeof(BMPInfoHeader_t), 1, fp);

    if((0 !=memcmp((char*)&bmpfileheader.Type, "BM", 2))
      &&(0 !=memcmp((char*)&bmpfileheader.Type, "MB", 2)))
    {
        fclose(fp);
        return GETPICTURE_NOTBMP;
    }

    CreateDirectoryK(dir);

    bmpfilenum = (bmpinfoheader.Height + coord->Height - 1)/coord->Height;
    *splitnum = bmpfilenum;

    if(bmpinfoheader.BitCount == 32)
    {
        uint16 heightremain = bmpinfoheader.Height%coord->Height;

        bmpfilelen = bmpposition + 4*coord->Width*coord->Height;
        filecontent = malloc(sizeof(char*) * bmpfilenum);
        for (i = 0; i < bmpfilenum; i++)
            filecontent[i] = malloc(bmpfilelen);

        if (bmpinfoheader.Height%coord->Height != 0)
        {
            memset(filecontent[bmpfilenum-1] + bmpposition, 0, 4*coord->Width*coord->Height);

            for (j = 0; j < heightremain; j++)
            {
                fseek(fp, bmpposition+ j*4*bmpinfoheader.Width ,SEEK_SET);
                fread(filecontent[bmpfilenum-1] + bmpposition + (coord->Height - heightremain + j)*4*coord->Width, 1, 4*coord->Width, fp);
            }

            for (i = 0; i < bmpinfoheader.Height/coord->Height; i++)
            {
                for (j = 0; j < coord->Height; j++)
                {
                    fseek(fp, bmpposition+ (i*coord->Height + heightremain + j)*4*bmpinfoheader.Width ,SEEK_SET);
                    fread(filecontent[bmpfilenum-1-i-1] + bmpposition + j*4*coord->Width, 1, 4*coord->Width, fp);
                }
            }
        }
        else
        {
            for(i = 0; i < bmpinfoheader.Height/coord->Height; i++)
            {
                for (j = 0; j < coord->Height; j++)
                {
                    fseek(fp, bmpposition+ (j+(i*coord->Height))*4*bmpinfoheader.Width ,SEEK_SET);
                    fread(filecontent[bmpfilenum-i-1] + bmpposition + j*4*coord->Width, 1, 4*coord->Width, fp);
                }
            }
        }
    }
    else if(bmpinfoheader.BitCount == 24)
    {
        uint16 heightremain = bmpinfoheader.Height%coord->Height;
        uint8 srclineremain = ((3*bmpinfoheader.Width)%4 ? 4 - (3*bmpinfoheader.Width)%4 : 0);
        uint8 dstlineremain = ((3*coord->Width)%4 ? 4 - (3*coord->Width)%4 : 0);
        uint16 srclinelenbyte = 3*bmpinfoheader.Width + srclineremain;
        uint16 dstlinelenbyte = 3*coord->Width + dstlineremain;

        bmpfilelen = bmpposition + (srclinelenbyte * coord->Height);
        filecontent = malloc(sizeof(char*) * bmpfilenum);
        for (i = 0; i < bmpfilenum; i++)
            filecontent[i] = malloc(bmpfilelen);

        if (bmpinfoheader.Height%coord->Height != 0)
        {
            memset(filecontent[bmpfilenum-1] + bmpposition, 0, 3*coord->Width*coord->Height);

            for (j = 0; j < heightremain; j++)
            {
                fseek(fp, bmpposition+ j*srclinelenbyte ,SEEK_SET);
                fread(filecontent[bmpfilenum-1] + bmpposition + (coord->Height - heightremain + j)*dstlinelenbyte, 1, dstlinelenbyte, fp);
            }

            for (i = 0; i < bmpinfoheader.Height/coord->Height; i++)
            {
                for (j = 0; j < coord->Height; j++)
                {
                    fseek(fp, bmpposition+ (i*coord->Height + heightremain + j)*srclinelenbyte ,SEEK_SET);
                    fread(filecontent[bmpfilenum-1-i-1] + bmpposition + j*dstlinelenbyte, 1, dstlinelenbyte, fp);
                }
            }
        }
        else
        {
            for(i = 0; i < bmpinfoheader.Height/coord->Height; i++)
            {
                for (j = 0; j < coord->Height; j++)
                {
                    fseek(fp, bmpposition+ (j+(i*coord->Height))*srclinelenbyte ,SEEK_SET);
                    fread(filecontent[bmpfilenum-i-1] + bmpposition + j*dstlinelenbyte, 1, dstlinelenbyte, fp);
                }
            }
        }
    }
    else
    {
        fclose(fp);
        return GETPICTURE_NOT24OR32BITS;
    }

    FILE *cfp;
    char path[PATH_LEN];
    bmpinfoheader.Width =coord->Width;
    bmpinfoheader.Height =coord->Height;
    for (i = 0; i < bmpfilenum; i++)
    {
        memcpy(filecontent[i], &bmpfileheader, sizeof(BMPFileHeader_t));
        memcpy(filecontent[i] + sizeof(BMPFileHeader_t), &bmpinfoheader, sizeof(BMPInfoHeader_t));

        if (dir[strlen(dir) - 1] != '/')
            sprintf(path, "%s/%s_%d.bmp", dir, startname, i);
        else
            sprintf(path, "%s%s_%d.bmp", dir, startname, i);

        dprif("CreatSplitBMPFile:%s\n", path);

        if ((cfp = fopen(path, "wb")) == NULL)
        {
            for (i = 0; i < bmpfilenum; i++)
            {
                if (filecontent[i] != NULL)
                    free(filecontent[i]);
            }
            if (filecontent[i] != NULL)
                free(filecontent);

            fclose(fp);
            return GETPICTURE_NOFILE;
        }

        fwrite(filecontent[i], sizeof(char), bmpfilelen, cfp);
        fclose(cfp);
    }

    for (i = 0; i < bmpfilenum; i++)
    {
        if (filecontent[i] != NULL)
            free(filecontent[i]);
    }
    if (filecontent[i] != NULL)
        free(filecontent);

    fclose(fp);

    return GETPICTURE_SUCCESS;
}



/*
 *******************************************************************************
 * author:
 * date:
 * description: 1,1
                01
                aim->Buf
 *******************************************************************************
 */
int Protocol_SendFrameContent(const SendIdeology_t *send, ProtocolAimData_t *aim, Record_t *record)
{
    uint16 framelimitlength = 0;
    ProtocolSrcData_t protocolsrcdata;
    static RCFileArch_t rcfilearch;
    char filepathtmp[PATH_LEN];
    char contentbuf[PROTOCOL_AIMCONTENTLENGTH];

    memset(&protocolsrcdata, 0, sizeof(ProtocolSrcData_t));
    memset(&filepathtmp, 0, PATH_LEN);
    memset(&contentbuf, 0, PROTOCOL_AIMCONTENTLENGTH);

    if ((send->FrameLimitLength != 512)
      && (send->FrameLimitLength != 256)
      && (send->FrameLimitLength != 1024))
        framelimitlength = 512 - (24 + FILE_HEADER_LEN);
    else
        framelimitlength = send->FrameLimitLength - (24 + FILE_HEADER_LEN);
    protocolsrcdata.AimAddress = send->AimAddress;
    protocolsrcdata.SrcAddress = send->SrcAddress;

    aim->Length = 0;
    switch(send->CommandType)
    {
    case COMMAND_OPEN_SCREEN:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_DISPLAY_SET;
            sprintf(strtmp, "S1");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_CLOSE_SCREEN:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_DISPLAY_SET;
            sprintf(strtmp, "S0");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_SEARCH_SCREEN:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_INFO_READ;
            sprintf(strtmp, "P,V,D,d,w,b,T,I,i,S,W,H,c,g,");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_COMMUNICATION_TEST:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_INFO_READ;
            sprintf(strtmp, "P,V,D,d,w,b,T,I,i,S,W,H,c,g,");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_SAVE_DISPLAY_CONFIG:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_DISPLAY_SAVE;
            sprintf(strtmp, "SAVE_DISPLAY_PARA");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_CLEAR_CONTENT:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_CONTENT_CLEAR;
            sprintf(strtmp, "CLEAR_CONTENT");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_SAVE_PROGRAM_CONTENT:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_CONTENT_SAVE;
            sprintf(strtmp, "SAVE_CONTENT");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_ADJUST_TIME:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_DISPLAY_SET;
            sprintf(strtmp, "D%d-%d-%d-%d,T%d:%d:%d",
                send->AdjustTime.Year, send->AdjustTime.Month, send->AdjustTime.Day,send->AdjustTime.Week,
                send->AdjustTime.Hour, send->AdjustTime.Min, send->AdjustTime.Sec);
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_ADJUST_BRIGHT:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_DISPLAY_SET;
            sprintf(strtmp, "l0,L%02d", send->BrightLevel);
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_GET_WIFI_CONFIG:
        {
            //8089UDP
            uint8 wifisend[10] = {0x02,0x00,0x0A,0x00,0x00,0x00,0xF3,0xFF,0xFF,0xFF};
            aim->Length = 10;
            memcpy(aim->Buf, wifisend, aim->Length);
            return SEND_COMPLETE;
        }
    case COMMAND_SET_WIFI_CONFIG:
        {
            if(0 != strcmp(send->WifiCfg.Password, send->WifiCfg.PwConfim))
            {
                dprif("Two password input is not the same\n");
                return -3;
            }
            if(!strncmp(send->WifiCfg.Name, "KL_",3))
            {
                uint8 headbuf[] = {0x03,0x00,0xA9,0x00,0x00,0x00,0x53,0xFF};
                memcpy(&aim->Buf[0],headbuf,8);
                Protocol_CreateWifiInfoAimBuffer((WifiConfig_t *)&send->WifiCfg, send->WifiSrc,aim);
            }
            else if(!strncmp(send->WifiCfg.Name, "KLX",3))
            {
                char strtmp[64];
                memset(strtmp,0,64);
                protocolsrcdata.FrameType = PROTOCOL_WIFI_SET;//17

                sprintf(strtmp, "%c%s%c%s",
                    strlen(send->WifiCfg.Name),send->WifiCfg.Name,
                    strlen(send->WifiCfg.Password), send->WifiCfg.Password);
                protocolsrcdata.Length = strlen(strtmp);
                protocolsrcdata.Buf = strtmp;
                Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            }
            else
            {
                dprif("Wifi Name Error\n");
                return -1;
            }

            return SEND_COMPLETE;
        }
    case COMMAND_WIFI_RESET:
        {
            uint8 wifisend[10] = {0x01,0x00,0x0A,0x00,0x00,0x00,0xF4,0xFF,0xFF,0xFF};
            aim->Length = 10;
            memcpy(aim->Buf, wifisend, aim->Length);
            return SEND_COMPLETE;
        }
    case COMMAND_GET_BASESCREEN_CONFIG:
        {
            char strtmp[64];
            protocolsrcdata.FrameType = PROTOCOL_BASESCREEN_GET;
            sprintf(strtmp, "GET_SCREEN");
            protocolsrcdata.Length = strlen(strtmp);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_SET_BASESCREEN_CONFIG:
        {
            char strtmp[128];
            memset(strtmp,0,128);
            protocolsrcdata.FrameType = PROTOCOL_BASESCREEN_SET;
            #if 0
            sprintf(strtmp, "%x%d%d%d%d%d%d%d%s\n",
            send->ScreenBaseCfg.Setflag,send->ScreenBaseCfg.StructLength,send->ScreenBaseCfg.Width,
            send->ScreenBaseCfg.Heigh,send->ScreenBaseCfg.ScanTimes,send->ScreenBaseCfg.ScreenLine,
            send->ScreenBaseCfg.Reserve1,send->ScreenBaseCfg.Reserve2,send->ScreenBaseCfg.ScreenMode);
            #endif
            memcpy(strtmp,&send->ScreenBaseCfg,sizeof(ScreenBaseConfig_t));
            protocolsrcdata.Length = sizeof(ScreenBaseConfig_t);
            protocolsrcdata.Buf = strtmp;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            return SEND_COMPLETE;
        }
    case COMMAND_UPDATE_CODE_UCF:
        {
            //UCF
            uint16 length;
            if (Protocol_DetectFileExist(send->FileDirectory, "ALL.ucf", "ALL.UCF", filepathtmp) != 0)
                return -1;
            if (Protocol_FileToContentBuffer(filepathtmp, send->FrameNumber*framelimitlength, framelimitlength, contentbuf, &length) != 0)
                return -2;
            protocolsrcdata.FrameType = PROTOCOL_UPDATEFILE_SET;
            protocolsrcdata.Length = length;
            protocolsrcdata.Buf = contentbuf;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            dprif("Length %d\n", protocolsrcdata.Length);
            if (length - FILE_HEADER_LEN != framelimitlength)
                return SEND_COMPLETE;
            else
                return SEND_RUNING;
        }
    case COMMAND_NETWORK_CONFIG:
        {
            uint16 length;
            if (Protocol_DetectFileExist(send->FileDirectory, "ALL.ncx", "ALL.ncf", filepathtmp) != 0)
                return -1;
            if (Protocol_FileToContentBuffer(filepathtmp, 0, framelimitlength, contentbuf, &length) != 0)
                return -2;
            protocolsrcdata.FrameType = PROTOCOL_NETPARAFILE_SET;
            protocolsrcdata.Length = length;
            protocolsrcdata.Buf = contentbuf;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            dprif("Length %d\n", protocolsrcdata.Length);
            return SEND_COMPLETE;
        }
    case COMMAND_DISPLAY_CONFIG:
        {
            uint16 length;
            if (Protocol_DetectFileExist(send->FileDirectory, "ALL.dcx", "ALL.dcf", filepathtmp) != 0)
                return -1;
            if (Protocol_FileToContentBuffer(filepathtmp, 0, framelimitlength, contentbuf, &length) != 0)
                return -2;
            protocolsrcdata.FrameType = PROTOCOL_DISPLAYFILE_SET;
            protocolsrcdata.Length = length;
            protocolsrcdata.Buf = contentbuf;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            dprif("Length %d\n", protocolsrcdata.Length);
            return SEND_COMPLETE;
        }
    case COMMAND_SCREEN_CONFIG:
        {
            uint16 length;
            if (Protocol_DetectFileExist(send->FileDirectory, "ALL.scx", "ALL.scf", filepathtmp) != 0)
            {
                dprif("filepathtmp %s\n",filepathtmp);
                return -1;
            }
            if (Protocol_FileToContentBuffer(filepathtmp, send->FrameNumber*framelimitlength, framelimitlength, contentbuf, &length) != 0)
            {
                dprif("filepathtmp %s\n",filepathtmp);
                return -2;
            }
            protocolsrcdata.FrameType = PROTOCOL_PARAFILE_SET;
            protocolsrcdata.Length = length;
            protocolsrcdata.Buf = contentbuf;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            dprif("Length %d\n", protocolsrcdata.Length);
            return SEND_COMPLETE;
        }
    case COMMAND_SCREEN_CONFIG_EXTREN:
        {
            uint16 length;
            if (Protocol_DetectFileExist(send->FileDirectory, "ALL.sex", "ALL.sef", filepathtmp) != 0)
            {
                dprif("filepathtmp %s\n",filepathtmp);
                return -1;
            }

            if (Protocol_FileToContentBuffer(filepathtmp, send->FrameNumber*framelimitlength, framelimitlength, contentbuf, &length) != 0)
            {
                dprif("filepathtmp %s\n",filepathtmp);
                return -2;
            }
            protocolsrcdata.FrameType = PROTOCOL_SCANDOTFILE_SET;
            protocolsrcdata.Length = length;
            protocolsrcdata.Buf = contentbuf;
            Protocol_CreateAimBuffer(&protocolsrcdata, aim);
            dprif("1234\n");
            dprif("filepathtmp %s\n",filepathtmp);
            dprif("Length %d\n", protocolsrcdata.Length);
            if (length - FILE_HEADER_LEN != framelimitlength)
                return SEND_COMPLETE;
            else
                return SEND_RUNING;
        }
    case COMMAND_PROGRAM_CONTENT:
        {
            FileHeader_t fileheader;
            FILE *fp;
            uint8 flag = 0;
            uint16 length;
            uint32 offset;
            uint32 framecnt = 0;
            int i;

            if (Protocol_DetectFileExist(send->FileDirectory, "ALL.rcx", "ALL.rcf", filepathtmp) != 0)
                return -1;

            protocolsrcdata.FrameType = PROTOCOL_CONTENTFILE_SET;

            if((fp = fopen(filepathtmp, "rb")) == NULL)
            {
                dprif("file path %s error\n",filepathtmp);
                return -2;
            }

            fseek(fp, 0, SEEK_SET);
            fread(&fileheader, sizeof(FileHeader_t), 1, fp);
            fread(&rcfilearch.Number, sizeof(uint16), 1, fp);

            if (send->FrameNumber == 0)
            {
                dprif("file list number %d\n", rcfilearch.Number);
                for(i = 0; i < rcfilearch.Number; i++)
                {
                    fread(&rcfilearch.FileList[i], sizeof(FileList_t), 1, fp);
                    dprif("rcfilearch.FileList[%d].Name %s\n", i, rcfilearch.FileList[i].Name);
                    dprif("rcfilearch.FileList[%d].ContentOffset %d\n", i, rcfilearch.FileList[i].ContentOffset);
                }

                for(i = 0; i < rcfilearch.Number; i++)
                {
                    fseek(fp, rcfilearch.FileList[i].ContentOffset, SEEK_SET);
                    fread(&fileheader, sizeof(FileHeader_t), 1, fp);
                    rcfilearch.FileSize[i] = fileheader.Length;
                    dprif("rcfilearch.FileSize[%d] %d\n ", i, rcfilearch.FileSize[i]);
                }
            }

            for(i = 0; i < rcfilearch.Number; i++)
            {
                offset = 0;
                flag = 0;
                for (;;)
                {
                    if (++framecnt == send->FrameNumber + 1)
                        flag = 0x5a;

                    if (Protocol_FunnyToContentBuffer(fp, rcfilearch.FileList[i].Name,
                                            rcfilearch.FileList[i].ContentOffset, rcfilearch.FileSize[i], flag,
                                            offset, framelimitlength, contentbuf, &length) == 0)
                    {
                        offset += ((length == 0) ? 0 : (length - FILE_HEADER_LEN));
                        if ((length < FILE_HEADER_LEN) || (length - FILE_HEADER_LEN < framelimitlength))
                            break;
                    }
                    else
                    {
                        fclose(fp);
                        return -2;
                    }

                    if (flag)
                        break;
                }

                if (flag)
                {
                    dprif("Name:%s, Offset:%d, Size:%d, Sendlen:%d\n",
                        rcfilearch.FileList[i].Name,
                        rcfilearch.FileList[i].ContentOffset,
                        rcfilearch.FileSize[i],
                        length);
                    protocolsrcdata.Length = length;
                    protocolsrcdata.Buf = contentbuf;
                    Protocol_CreateAimBuffer(&protocolsrcdata, aim);
                    //Protocol_PrintBuffer(aim->Buf, aim->Length);
                    if ((length - FILE_HEADER_LEN != framelimitlength) && (i == rcfilearch.Number - 1)) {
                        fclose(fp);
                        return SEND_COMPLETE;
                    } else {
                        fclose(fp);
                        return SEND_RUNING;
                    }
                }
            }

            fclose(fp);
            return SEND_RUNING;
        }
    case COMMAND_ALL_CONTENT:
    default:
        dprif("protocol type error\n");
        break;
    }

    return -3;
}


ProtocolAimData_t Protocol_ReturnFrameContent(const SendIdeology_t *send, Record_t *record)
{
    ProtocolAimData_t aim;
    memset(&aim,0,sizeof(ProtocolAimData_t));

    aim.ret = Protocol_SendFrameContent(send, &aim, record);
    return aim;
}

/*
ProtocolAimData_t Protocol_ReturnFrameContent(const SendIdeology_t *send)
{
    ProtocolAimData_t aim;
    Record_t record;
    memset(&aim,0,sizeof(ProtocolAimData_t));
    memset(&record,0,sizeof(Record_t));
    dprif("%d\n",send->CommandType);
    aim.ret = Protocol_SendFrameContent(send, &aim, &record);

    return aim;
}
*/
/*
 *******************************************************************************
 * author:
 * date:
 * description:bufferbuffer
               0-13

 *******************************************************************************
 */
int Protocol_ParseReplyBuffer(const char *buf, uint16 len, ProtocolAimData_t *aim)
{
    uint16 i;
    ReplyParseInfo_t rcvinfo;

    memset(&rcvinfo, 0, sizeof(ReplyParseInfo_t));
    rcvinfo.State = RS232_RECEIVE_WAIT0;

    for (i = 0; i < len; i++)
    {
        Protocol_ReceiveParseData(&rcvinfo, buf[i]);
        //dprif("reply protocol state %d\n", rcvinfo.State);
        if (rcvinfo.State == RS232_RECEIVE_SUCCEED0)
        {
            dprif("reply protocol OK!\n");
            aim->Length = rcvinfo.Sum;
            memcpy(aim->Buf, rcvinfo.RcvBuf, rcvinfo.Sum);
            return 0;
        }
    }

    aim->Length = 0;
    return -1;
}


