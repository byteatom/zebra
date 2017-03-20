#ifndef __PARSE_PROTOCOL_H__
#define __PARSE_PROTOCOL_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

#include "parse_program.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */

//
#define COMMAND_OPEN_SCREEN                 (1)
#define COMMAND_CLOSE_SCREEN                (2)
#define COMMAND_SEARCH_SCREEN               (3)
#define COMMAND_COMMUNICATION_TEST          (4)
#define COMMAND_ADJUST_BRIGHT               (5)
#define COMMAND_SAVE_DISPLAY_CONFIG         (6)
#define COMMAND_CLEAR_CONTENT               (7)
#define COMMAND_SAVE_PROGRAM_CONTENT        (8)
#define COMMAND_ADJUST_TIME                 (9)
#define COMMAND_DISPLAY_CONFIG              (10)     //dcf
#define COMMAND_NETWORK_CONFIG              (11)     //ncf
#define COMMAND_SCREEN_CONFIG               (12)     //scf
#define COMMAND_SCREEN_CONFIG_EXTREN        (13)     //sef
#define COMMAND_PROGRAM_CONTENT             (14)     //rcf
#define COMMAND_ALL_CONTENT                 (15)
#define COMMAND_GET_WIFI_CONFIG             (16)
#define COMMAND_SET_WIFI_CONFIG             (17)
#define COMMAND_GET_BASESCREEN_CONFIG       (18)
#define COMMAND_SET_BASESCREEN_CONFIG       (19)
#define COMMAND_UPDATE_CODE_UCF             (20)
#define COMMAND_WIFI_RESET                  (21)

//
#define PROTOCOL_PARAFILE_SET                    0
#define PROTOCOL_PARAFILE_READ                   1
#define PROTOCOL_DISPLAYFILE_SET                 2
#define PROTOCOL_DISPLAYFILE_READ                3
#define PROTOCOL_CONTENTFILE_SET                 4
#define PROTOCOL_CONTENTFILE_READ                5
#define PROTOCOL_POWERONFILE_SET                 6
#define PROTOCOL_INFOFILE_READ                   7
#define PROTOCOL_UPDATEFILE_SET                  8
#define PROTOCOL_LOGFILE_READ                    9
#define PROTOCOL_SCANDOTFILE_SET                 10
#define PROTOCOL_PARAEXTFILE_READ                11
#define PROTOCOL_NETPARAFILE_SET                 12
#define PROTOCOL_NETPARAFILE_READ                13
#define PROTOCOL_WIFI_SET                        17
#define PROTOCOL_BASESCREEN_GET                  28
#define PROTOCOL_BASESCREEN_SET                  29
#define PROTOCOL_CONTENT_CLEAR                   20
#define PROTOCOL_CONTENT_SAVE                    21
#define PROTOCOL_DISPLAY_SET                     50
#define PROTOCOL_DISPLAY_READ                    51
#define PROTOCOL_DISPLAY_SAVE                    52
#define PROTOCOL_INFO_SET                        53
#define PROTOCOL_INFO_READ                       54
#define PROTOCOL_TEMPERATURE_SET                 55
#define PROTOCOL_CHARACTER_SET                   57
#define PROTOCOL_CHARACTER_NOREPLY_SET           58
#define PROTOCOL_UARTCTRL_SET                    59
#define PROTOCOL_SCANDOT_TEST                    60
#define PROTOCOL_ENTERMOZDE_SET                  61
#define PROTOCOL_ENTERMOZDE_READ                 62
#define PROTOCOL_TEST_UART                       80
#define PROTOCOL_TEST_RTC                        81
#define PROTOCOL_SET_BUTTON                      98
#define PROTOCOL_RESET_DEVICE                    99

//buf
#define PROTOCOL_HEADERLENGTH_SIZE               3
#define PROTOCOL_AIMADDRESSLENGTH_SIZE           4
#define PROTOCOL_SRCADDRESSLENGTH_SIZE           4
#define PROTOCOL_FRAMETYPELENGTH_SIZE            4
#define PROTOCOL_FRAMELENGTH_SIZE                4
#define PROTOCOL_TAILLENGTH_SIZE                 3
#define PROTOCOL_VERIFYLENGTH_SIZE               2

//
#define RS232_RECEIVE_WAIT0             0   //
#define RS232_RECEIVE_HEAD0             1   //
#define RS232_RECEIVE_HEAD_SUM0         3   //
#define RS232_RECEIVE_DESADDR0          2   //
#define RS232_RECEIVE_DESADDR_SUM0      4   //
#define RS232_RECEIVE_SRCADDR0          3   //
#define RS232_RECEIVE_SRCADDR_SUM0      4   //
#define RS232_RECEIVE_DATATYPE0         4   //
#define RS232_RECEIVE_DATATYPE_SUM0     4   //
#define RS232_RECEIVE_DATA_LENGHT0      5   //
#define RS232_RECEIVE_DATALENGHT_SUM0   4   //
#define RS232_RECEIVE_DATA0             6   //
#define RS232_RECEIVE_VERIFY0           7   //
#define RS232_RECEIVE_VERIFY_SUM0       2   //
#define RS232_RECEIVE_TRAIL0            8   //
#define RS232_RECEIVE_TRAIL_SUM0        3   //
#define RS232_RECEIVE_FAIL0             254 //
#define RS232_RECEIVE_SUCCEED0          255 //


//RCX
#define FILE_NAME_LEN                            16
#define FILE_HEADER_LEN                          (FILE_NAME_LEN + sizeof(uint32))
#define FILE_NUMBER                              1024
#define CONTENT_BUFFER_LEN                       1024
#pragma pack(push, 1)
typedef struct _FileList_
{
	uint32 ContentOffset;
	char Name[FILE_NAME_LEN];
} FileList_t;
#pragma pack(pop)

typedef struct _RCFileArch_
{
	uint16 Number;
	FileList_t FileList[FILE_NUMBER];
	uint32 FileSize[FILE_NUMBER];
}RCFileArch_t;

//AIMSRC
#define PROTOCOL_AIMCONTENTLENGTH           1200
typedef struct _ProtocolAimData_
{
	uint16 Length;
	int32 ret;      //
	uint8 Buf[PROTOCOL_AIMCONTENTLENGTH];
}ProtocolAimData_t;

typedef struct _ProtocolSrcData_
{
	uint16 AimAddress;
	uint16 SrcAddress;
	uint16 FrameType;
	uint16 Length;
	uint8  *Buf;
}ProtocolSrcData_t;

//
typedef struct _ReplyParseInfo_
{
	uint16 State;                   //0-  0xff-
	uint16 Sum;                     //
	uint16 Couter;                  //
	uint16 SrcAddress;              //
	uint16 RcvAddress;              //
	uint8  SrcVerify;               //
	uint8  MyVerify;                //
	uint16 OverTime;                //
	uint16 FrameType;               //
	uint8  RcvBuf[PROTOCOL_AIMCONTENTLENGTH];    //
}ReplyParseInfo_t;

//wifi
typedef struct _WifiConfig_
{
	uint8 Name[32];                     //wifi
	uint8 Password[16];
	uint8 PwConfim[16];
}WifiConfig_t;

//
typedef struct _ScreenBaseConfig_
{
	uint16 Setflag;          //
	uint16 StructLength;
	uint16 Width;               //
	uint16 Heigh;               //
	uint8 ColorThreshold;
	uint8 OEPolar;
	uint8 DataPolar;
	uint8 Frequency;            //
	uint8 LineBlank;           //
	uint8 Reserve1;             //
	char ScreenMode[50] ;         //
}ScreenBaseConfig_t;

//
typedef struct _SendIdeology_
{
	char FileDirectory[PATH_LEN];
	uint16 FrameLimitLength;

	uint16 AimAddress;
	uint16 SrcAddress;
	uint16 CommandType;    //

	uint16 FrameNumber;    //
	uint16 BrightLevel;
	DateTime_t AdjustTime;
	uint8 *WifiSrc;
	WifiConfig_t WifiCfg;
	ScreenBaseConfig_t ScreenBaseCfg;
}SendIdeology_t;

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define SEND_RUNING         (0)
#define SEND_COMPLETE       (1)

int Protocol_CreateAimBuffer(const ProtocolSrcData_t *src, ProtocolAimData_t *aim);
void Protocol_ReceiveParseData(ReplyParseInfo_t *rcvinfo, uint8 rcvdata);
int Protocol_FileToContentBuffer(const char *filepath, uint32 offset, uint32 size, char *ctnbuf, uint16 *ctnlen);
int Protocol_FunnyToContentBuffer(FILE *fp, const char *filename, uint32 position, uint32 filelength, uint8 flag, uint32 offset, uint32 size, char *ctnbuf, uint16 *ctnlen);
int Protocol_DetectFileExist(const char *filedir, const char *file1, const char *file2, char *filepath);
void Protocol_PrintBuffer(const unsigned char *buf, uint16 len);
uint16 WifiCalcSum(void *data, uint32 len);
int CheckWifiInfoSrcBuffer(uint8 *src);
int Protocol_CreateWifiInfoAimBuffer(WifiConfig_t *wifi, uint8 *src, ProtocolAimData_t *aim);

int Protocol_SendFrameContent(const SendIdeology_t *send, ProtocolAimData_t *aim, Record_t *record);
int Protocol_ParseReplyBuffer(const char *buf, uint16 len, ProtocolAimData_t *aim);

#endif // __PARSE_PROTOCOL_H__

