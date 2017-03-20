#include "kaler_config.h"
#include "../json/json.h"
#include "parse_protocol.h"

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
const uint8 PROTOCOL_HEADER[PROTOCOL_HEADERLENGTH_SIZE] = "KLB";
const uint8 PROTOCOL_TAIL[PROTOCOL_TAILLENGTH_SIZE] = "KLE";
const char DecToHex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

int Protocol_CreateAimBuffer(const ProtocolSrcData_t *src, ProtocolAimData_t *aim)
{
    int i = 0;
    uint16 length = 0;
    uint16 lengthsum = 0;
    uint8  verifysum = 0;

    length = src->Length;

    lengthsum = PROTOCOL_HEADERLENGTH_SIZE
                + PROTOCOL_AIMADDRESSLENGTH_SIZE
                + PROTOCOL_SRCADDRESSLENGTH_SIZE
                + PROTOCOL_FRAMETYPELENGTH_SIZE
                + PROTOCOL_FRAMELENGTH_SIZE
                + length
                + PROTOCOL_TAILLENGTH_SIZE;

    //
    for(i = 0; i < PROTOCOL_HEADERLENGTH_SIZE; i++)
    {
        aim->Buf[i] = PROTOCOL_HEADER[i];
    }
    //
    aim->Buf[i++] = src->AimAddress/1000 +'0';
    aim->Buf[i++] = src->AimAddress%1000/100 +'0';
    aim->Buf[i++] = src->AimAddress%100/10 +'0';
    aim->Buf[i++] = src->AimAddress%10 +'0';
    //
    aim->Buf[i++] = src->SrcAddress/1000 +'0';
    aim->Buf[i++] = src->SrcAddress%1000/100 +'0';
    aim->Buf[i++] = src->SrcAddress%100/10 +'0';
    aim->Buf[i++] = src->SrcAddress%10 +'0';
    //
    aim->Buf[i++] = src->FrameType/1000 +'0';
    aim->Buf[i++] = src->FrameType%1000/100 +'0';
    aim->Buf[i++] = src->FrameType%100/10 +'0';
    aim->Buf[i++] = src->FrameType%10 +'0';
    //
    aim->Buf[i++] = src->Length/1000 +'0';
    aim->Buf[i++] = src->Length%1000/100 +'0';
    aim->Buf[i++] = src->Length%100/10 +'0';
    aim->Buf[i++] = src->Length%10 +'0';
    //
    memcpy((uint8*)&aim->Buf[i], (uint8*)src->Buf, length);
    //
    for(i = 0; i < PROTOCOL_TAILLENGTH_SIZE; i++)
    {
        aim->Buf[lengthsum - PROTOCOL_TAILLENGTH_SIZE + i] = PROTOCOL_TAIL[i];
    }
    //
    for(i = 0; i < lengthsum; i++)
    {
        verifysum += aim->Buf[i];
    }
    //
    aim->Buf[lengthsum] = DecToHex[verifysum/16];
    aim->Buf[lengthsum+1] = DecToHex[verifysum%16];
    aim->Buf[lengthsum+2] = '\0';

    aim->Length = lengthsum + PROTOCOL_VERIFYLENGTH_SIZE;

    return 0;
}


/*
*********************************************************************************************************
*
*********************************************************************************************************
*/
void Protocol_ReceiveParseData(ReplyParseInfo_t *rcvinfo, uint8 rcvdata)
{
    switch(rcvinfo->State)
    {
    case RS232_RECEIVE_WAIT0:
        rcvinfo->Couter =0;//
        rcvinfo->MyVerify =0;//
        rcvinfo->State =RS232_RECEIVE_HEAD0;
    case RS232_RECEIVE_HEAD0:
        {
            if(rcvdata !=PROTOCOL_HEADER[rcvinfo->Couter])
            {//
                rcvinfo->State =RS232_RECEIVE_WAIT0;//'P', -  -
            }
            else
            {//
                rcvinfo->MyVerify +=rcvdata;
                if(++rcvinfo->Couter >=RS232_RECEIVE_HEAD_SUM0)
                {//,
                    rcvinfo->State =RS232_RECEIVE_DESADDR0;
                    rcvinfo->RcvAddress =0;
                    rcvinfo->Couter =0;
                }
            }
        }
        break;
    case RS232_RECEIVE_DESADDR0:
        {
            rcvinfo->MyVerify +=rcvdata;
            rcvinfo->RcvAddress =(rcvinfo->RcvAddress*10) +rcvdata -'0';
            if(++rcvinfo->Couter >=RS232_RECEIVE_DESADDR_SUM0)
            {
                if((1 ==rcvinfo->RcvAddress) || (0 ==rcvinfo->RcvAddress)
                    ||(rcvinfo->RcvAddress ==9999))//
                {//,
                    rcvinfo->State =RS232_RECEIVE_SRCADDR0;
                    rcvinfo->Couter =0;
                    rcvinfo->SrcAddress =0;
                }
                else
                {//
                    rcvinfo->State =RS232_RECEIVE_WAIT0;
                }
            }
        }
        break;
    case RS232_RECEIVE_SRCADDR0:
        {//
            rcvinfo->MyVerify +=rcvdata;
            rcvinfo->SrcAddress =(rcvinfo->SrcAddress*10) +rcvdata -'0';
            if(++rcvinfo->Couter >=RS232_RECEIVE_SRCADDR_SUM0)
            {
                rcvinfo->State =RS232_RECEIVE_DATATYPE0;
                rcvinfo->Couter =0;
                rcvinfo->FrameType =0;
            }
        }
        break;
    case RS232_RECEIVE_DATATYPE0:
        {//
            rcvinfo->MyVerify +=rcvdata;
            rcvinfo->FrameType =(rcvinfo->FrameType*10) +rcvdata -'0';
            if(++rcvinfo->Couter >=RS232_RECEIVE_DATATYPE_SUM0)
            {
                rcvinfo->State =RS232_RECEIVE_DATA_LENGHT0;
                rcvinfo->Couter =0;
                rcvinfo->Sum =0;
            }
        }
        break;
    case RS232_RECEIVE_DATA_LENGHT0:
        {//
            rcvinfo->MyVerify +=rcvdata;
            rcvinfo->Sum =(rcvinfo->Sum*10) +rcvdata -'0';
            if(++rcvinfo->Couter >=RS232_RECEIVE_DATALENGHT_SUM0)
            {
                rcvinfo->Couter =0;
                if(rcvinfo->Sum ==0)
                    rcvinfo->State =RS232_RECEIVE_TRAIL0;
                else if (rcvinfo->Sum >= CONTENT_BUFFER_LEN)
                    rcvinfo->State =RS232_RECEIVE_WAIT0;
                else
                    rcvinfo->State =RS232_RECEIVE_DATA0;
            }
        }
        break;
    case RS232_RECEIVE_DATA0:       //
        {//
            rcvinfo->MyVerify+=rcvdata;
            rcvinfo->RcvBuf[rcvinfo->Couter++] =rcvdata;//
            if(rcvinfo->Couter ==rcvinfo->Sum)
            {
                rcvinfo->RcvBuf[rcvinfo->Couter] = '\0';
                rcvinfo->RcvBuf[rcvinfo->Couter+1] = '\0';
                rcvinfo->Couter =0;
                rcvinfo->State =RS232_RECEIVE_TRAIL0;
            }
        }
        break;
    case RS232_RECEIVE_TRAIL0:      //
        {//
            if(rcvdata !=PROTOCOL_TAIL[rcvinfo->Couter])
            {
                rcvinfo->State =RS232_RECEIVE_WAIT0;
            }
            else
            {
                rcvinfo->MyVerify +=rcvdata;
                if(++rcvinfo->Couter >=RS232_RECEIVE_TRAIL_SUM0)
                {
                    rcvinfo->State =RS232_RECEIVE_VERIFY0;
                    rcvinfo->Couter =0;
                    rcvinfo->SrcVerify =0;
                }
            }
        }
        break;
    case RS232_RECEIVE_VERIFY0:     //
        {
            rcvinfo->SrcVerify =rcvinfo->SrcVerify*16;
            if((rcvdata <='9') &&(rcvdata >='0'))
                rcvinfo->SrcVerify +=rcvdata -'0';
            else if((rcvdata <='F') &&(rcvdata >='A'))
                rcvinfo->SrcVerify +=rcvdata -'A' +10;
            else if((rcvdata <='f') &&(rcvdata >='a'))
                rcvinfo->SrcVerify +=rcvdata -'a' +10;
            else
                rcvinfo->State=RS232_RECEIVE_WAIT0;

            if(++rcvinfo->Couter >=RS232_RECEIVE_VERIFY_SUM0)
            {
                if((rcvinfo->SrcVerify ==rcvinfo->MyVerify)
                    || (rcvinfo->SrcVerify == 0xff)){
                    rcvinfo->State=RS232_RECEIVE_SUCCEED0;   //
                }else{
                    rcvinfo->State=RS232_RECEIVE_WAIT0;     //
                }
            }
        }
        break;
    case RS232_RECEIVE_FAIL0:
        //rcvinfo->State=RS232_RECEIVE_WAIT0;
        break;
    case RS232_RECEIVE_SUCCEED0:
        //rcvinfo->State=RS232_RECEIVE_WAIT0;
        break;
    default:
        rcvinfo->State=RS232_RECEIVE_WAIT0;
        break;
    }

    rcvinfo->OverTime=0;             //
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int Protocol_FileToContentBuffer(const char *filepath, uint32 offset, uint32 size, char *ctnbuf, uint16 *ctnlen)
{
    FILE* fp = NULL;
    uint32 filelength;
    uint16 readsize;
    char filename[FILE_NAME_LEN];

    *ctnlen = 0;

    if((fp = fopen(filepath, "rb")) == NULL)
    {
        dprif("file path %s error\n", filepath);
        return -1;
    }

    filelength = GetFileSizeK(fp);
    memset(filename, 0, FILE_NAME_LEN);
    DetachFilePath(filepath, NULL, filename);

    if (offset + size > filelength)
        readsize = filelength - offset;
    else
        readsize = size;

    if ((offset > filelength) || (readsize > CONTENT_BUFFER_LEN - FILE_HEADER_LEN))
    {
        dprif("read offset or size error\n");
        fclose(fp);
        return -2;
    }

    fseek(fp, offset, SEEK_SET);
    fread(&ctnbuf[FILE_HEADER_LEN], readsize, 1, fp);
    *ctnlen = FILE_HEADER_LEN + readsize;
    memcpy(&ctnbuf[0], filename, FILE_NAME_LEN);
    memcpy(&ctnbuf[FILE_NAME_LEN], &offset, sizeof(uint32));

    fclose(fp);
    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int Protocol_FunnyToContentBuffer(FILE *fp, const char *filename, uint32 position, uint32 filelength, uint8 flag, uint32 offset, uint32 size, char *ctnbuf, uint16 *ctnlen)
{
    uint16 readsize;

    *ctnlen = 0;

    if (offset + size > filelength)
        readsize = filelength - offset;
    else
        readsize = size;

    if ((offset > filelength) || (readsize > CONTENT_BUFFER_LEN - FILE_HEADER_LEN))
    {
        dprif("read offset or size error\n");
        return -2;
    }

    if (flag)
    {
        fseek(fp, position + offset, SEEK_SET);
        fread(&ctnbuf[FILE_HEADER_LEN], readsize, 1, fp);
        memcpy(&ctnbuf[0], filename, FILE_NAME_LEN);
        memcpy(&ctnbuf[FILE_NAME_LEN], &offset, sizeof(uint32));
    }

    *ctnlen = FILE_HEADER_LEN + readsize;

    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int Protocol_DetectFileExist(const char *filedir, const char *file1, const char *file2, char *filepath)
{
    dprif("filedir %s\n",filedir);
    if (file1 != NULL)
    {
        if (filedir[strlen(filedir) - 1] != '/')
            sprintf(filepath, "%s/%s", filedir, file1);
        else
            sprintf(filepath, "%s%s", filedir, file1);
        dprif("filepath %s\n",filepath);
    }
    else
    {
        filepath[0] = '\0';
    }


    if (access(filepath, 0) == -1)
    {
        if (file2 != NULL)
        {
            if (filedir[strlen(filedir) - 1] != '/')
                sprintf(filepath, "%s/%s", filedir, file2);
            else
                sprintf(filepath, "%s%s", filedir, file2);
dprif("filepath %s\n",filepath);
            if (access(filepath, 0) == -1)
                return -1;
        }
        else
        {
            filepath[0] = '\0';
            return -1;
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
void Protocol_PrintBuffer(const unsigned char *buf, uint16 len)
{
#if 1
    printf("* Content : %s\n", buf);

    uint16 i;

    for (i = 0; i < len; i++)
        printf("%.2X ", buf[i]);
    printf("\n\n");

    for (i = 0; i < len; i++)
        printf("%c ", buf[i]);
    printf("\n\n");

#endif
}


/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
uint16 WifiCalcSum(void *data, uint32 len)
{
    uint32 cksum = 0;
    uint16 *p = data;
    while (len > 1)
    {
        cksum += *p++;
        len -=2;
    }
    if (len)
    {
        cksum += *(uint8 *)p;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return ~cksum;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CheckWifiInfoSrcBuffer(uint8 *src)
{
#define WIFI_ALLx
    if(0 == src)
    {
        dprif("Wifisrc Buffer is NULL\n");
        return -1;
    }

#ifdef WIFI_ALL
    if(((src[7]<<8) + src[6]) != WifiCalcSum(src,6))
    {
        dprif("WifiCommand Check Error\n");
        return -1;
    }
#else
    uint16 command = 0;
    uint8 chkbuf[][8] = {{0x02,0x00,0xA9,0x00,0x01,0x00,0x53,0xFF},
                         {0x03,0x00,0xA9,0x00,0x01,0x00,0x52,0xFF}};

    switch(src[0] + (src[1]<<8))
    {
        case 0x0002:
            command = 0;
            break;
        case 0x0003:
            command = 1;
            break;
        default:
            return -1;
    }
    //
    if(0 != strncmp(chkbuf[command],src,8))
    {
        dprif("Command Check Error\n");
        return -1;
    }
#endif

    //
    uint16 srclen = src[2]+(src[3]<<8);
    uint16 chksum = WifiCalcSum(src+8,src[2]+(src[3]<<8) - 10);
    dprif("Data Chksum Value = %x\n",chksum);
    if(((src[srclen - 1]<<8) + src[srclen - 2]) != chksum)
    {
        dprif("WifiData Check Error\n");
        return -1;
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
int Protocol_CreateWifiInfoAimBuffer(WifiConfig_t *wifi, uint8 *src, ProtocolAimData_t *aim)
{
    uint16 chksum = 0;
    if(0 != CheckWifiInfoSrcBuffer(src))
    {
        dprif("Recieve Buf Check Error\n");
        return -1;
    }

    aim->Length = src[2] + (src[3]<<8);
    dprif("aim->Length %x\n",aim->Length);
    memcpy(&aim->Buf[8],src+8,aim->Length-8);
    if(0xA9 == aim->Length)
    {
        memcpy(&aim->Buf[9],wifi->Name,32);
        memcpy(&aim->Buf[41],wifi->Password,16);
        memcpy(&aim->Buf[aim->Length - 32],wifi->Password,16);
        chksum = WifiCalcSum(&aim->Buf[8],aim->Length - 10);
        aim->Buf[aim->Length - 2] = chksum;
        aim->Buf[aim->Length - 1] = chksum>>8;
    }
    else
    {
        //
    }
    return 0;
}


