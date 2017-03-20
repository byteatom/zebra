#include "kaler_config.h"
#include "../json/json.h"
#include "parse_acftime.h"

#ifdef undprif
#undef dprif
#define dprif(fmt, ...)
#endif

//#define USE_IOS
#ifdef USE_IOS
#define BIT_DEPTH   32
#else
#define BIT_DEPTH   24
#endif

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFTimeConfig(const json_object *new_obj, ACFTime_t **acf, Record_t *record)
{
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;
	ACFTime_t *pacf;

	*acf = malloc(sizeof(ACFTime_t));
	memset(*acf, 0, sizeof(ACFTime_t));

	pacf = *acf;

	json_object_object_foreach((json_object *)new_obj, key, val)
	{
		if (!strcmp(key, "startX"))
		{
			vint = json_object_get_int(val);
			pacf->StartX = vint;
			dprif("StartX : %d\n", pacf->StartX);

		}
		else if (!strcmp(key, "startY"))
		{
			vint = json_object_get_int(val);
			pacf->StartY = vint;
			dprif("StartY : %d\n", pacf->StartY);
		}
		else if (!strcmp(key, "jetLag"))
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
		else if (!strcmp(key, "showYearH"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_YEARH;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_YEARH;

			dprif("BMask(showYearH) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showYearL"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_YEARL;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_YEARL;

			dprif("BMask(showYearL) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showMonth"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_MONTH;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_MONTH;

			dprif("BMask(showMonth) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showDay"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_DAY;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_DAY;

			dprif("BMask(showDay) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showWeek"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_WEEK;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_WEEK;

			dprif("BMask(showWeek) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showHour"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_HOUR;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_HOUR;

			dprif("BMask(showHour) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showMin"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_MIN;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_MIN;

			dprif("BMask(showMin) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showSec"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_SEC;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_SEC;

			dprif("BMask(showSec) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "showAPM"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
				pacf->BMask |= ACF_TIME_SHOW_APM;
			else
				pacf->BMask &= ~ACF_TIME_SHOW_APM;

			dprif("BMask(showAPM) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "timeMode"))
		{
			strcpy(vstring, json_object_get_string(val));
			if(!strcmp(vstring, "24Hours"))
				pacf->BMask |= ACF_TIME_24HOURS_MODE;
			else if(!strcmp(vstring, "12Hours"))
				pacf->BMask &= ~ACF_TIME_24HOURS_MODE;
			else
			{
				dprif("BMask(timeMode) key error\n");
			}

			dprif("BMask(timeMode) : %.4X\n", pacf->BMask);
		}
		else if (!strcmp(key, "dateFontWidth"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->DateFontWidth = vint;
				dprif("DateFontWidth : %d\n", pacf->DateFontWidth);
			}
			else
			{
				dprif("DateFontWidth error,current value %d\n", pacf->DateFontWidth);
			}
		}
		else if (!strcmp(key, "dateFontHeight"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->DateFontHeight = vint;
				dprif("DateFontHeight : %d\n", pacf->DateFontHeight);
			}
			else
			{
				dprif("DateFontHeight error,current value %d\n", pacf->DateFontHeight);
			}
		}
		else if (!strcmp(key, "datePictureWidth"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->DatePictureWidth = ((vint+15)/16) * 16;
				dprif("DatePictureWidth : %d\n", pacf->DatePictureWidth);
			}
			else
			{
				dprif("DatePictureWidth error,current value %d\n", pacf->DatePictureWidth);
			}
		}
		else if (!strcmp(key, "datePictureHeight"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->DatePictureHeight = vint;
				dprif("DatePictureHeight : %d\n", pacf->DatePictureHeight);
			}
			else
			{
				dprif("DatePictureHeight error,current value %d\n", pacf->DatePictureHeight);
			}
		}
		else if (!strcmp(key, "dateYearHX"))
		{
			vint = json_object_get_int(val);
			pacf->DateYearHX = vint;
			dprif("DateYearHX : %d\n", pacf->DateYearHX);
		}
		else if (!strcmp(key, "dateYearHY"))
		{
			vint = json_object_get_int(val);
			pacf->DateYearHY = vint;
			dprif("DateYearHY : %d\n", pacf->DateYearHY);
		}
		else if (!strcmp(key, "dateYearLX"))
		{
			vint = json_object_get_int(val);
			pacf->DateYearLX = vint;
			dprif("DateYearLX : %d\n", pacf->DateYearLX);
		}
		else if (!strcmp(key, "dateYearLY"))
		{
			vint = json_object_get_int(val);
			pacf->DateYearLY = vint;
			dprif("DateYearLY : %d\n", pacf->DateYearLY);

		}
		else if (!strcmp(key, "dateMonthX"))
		{
			vint = json_object_get_int(val);
			pacf->DateMonthX = vint;
			dprif("DateMonthX : %d\n", pacf->DateMonthX);
		}
		else if (!strcmp(key, "dateMonthY"))
		{
			vint = json_object_get_int(val);
			pacf->DateMonthY = vint;
			dprif("DateMonthY : %d\n", pacf->DateMonthY);
		}
		else if (!strcmp(key, "dateDayX"))
		{
			vint = json_object_get_int(val);
			pacf->DateDayX = vint;
			dprif("DateDayX : %d\n", pacf->DateDayX);
		}
		else if (!strcmp(key, "dateDayY"))
		{
			vint = json_object_get_int(val);
			pacf->DateDayY = vint;
			dprif("DateDayY : %d\n", pacf->DateDayY);
		}
		else if (!strcmp(key, "dateContentArray"))
		{
			int i;
			int arraylen = json_object_array_length((json_object *)val);

			if (arraylen != 10)
			{
				arraylen = 0;
				dprif("DateContent array number error set to 0\n");
			}

			dprif("DateContent arraylen %d\n", arraylen);
			for (i = 0; i < arraylen; i++)
			{
				strcpy(vstring, json_object_get_string(json_object_array_get_idx(val, i)));
				if (strlen(vstring) < PATH_LEN)
				{
					strcpy(pacf->DateContent[i], vstring);
					dprif("DateContent[%d] : %s\n", i, pacf->DateContent[i]);
				}
				else
				{
					dprif("DateContent[%d] critical error\n", i);
				}
			}
		}
		else if (!strcmp(key, "weekFontWidth"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->WeekFontWidth = vint;
				dprif("WeekFontWidth : %d\n", pacf->WeekFontWidth);
			}
			else
			{
				dprif("WeekFontWidth error,current value %d\n", pacf->WeekFontWidth);
			}
		}
		else if (!strcmp(key, "weekFontHeight"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->WeekFontHeight = vint;
				dprif("WeekFontHeight : %d\n", pacf->WeekFontHeight);
			}
			else
			{
				dprif("WeekFontHeight error,current value %d\n", pacf->WeekFontHeight);
			}
		}
		else if (!strcmp(key, "weekPictureWidth"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->WeekPictureWidth = ((vint+15)/16) * 16;
				dprif("WeekPictureWidth : %d\n", pacf->WeekPictureWidth);
			}
			else
			{
				dprif("WeekPictureWidth error,current value %d\n", pacf->WeekPictureWidth);
			}
		}
		else if (!strcmp(key, "weekPictureHeight"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->WeekPictureHeight = vint;
				dprif("WeekPictureHeight : %d\n", pacf->WeekPictureHeight);
			}
			else
			{
				dprif("WeekPictureHeight error,current value %d\n", pacf->WeekPictureHeight);
			}
		}
		else if (!strcmp(key, "weekX"))
		{
			vint = json_object_get_int(val);
			pacf->WeekX = vint;
			dprif("WeekX : %d\n", pacf->WeekX);
		}
		else if (!strcmp(key, "weekY"))
		{
			vint = json_object_get_int(val);
			pacf->WeekY = vint;
			dprif("WeekY : %d\n", pacf->WeekY);
		}
		else if (!strcmp(key, "weekContentArray"))
		{
			int i;
			int arraylen = json_object_array_length((json_object *)val);

			if (arraylen != 7)
			{
				arraylen = 0;
				dprif("WeekContent array number error set to 0\n");
			}

			dprif("WeekContent arraylen %d\n", arraylen);
			for (i = 0; i < arraylen; i++)
			{
				strcpy(vstring, json_object_get_string(json_object_array_get_idx(val, i)));
				if (strlen(vstring) < PATH_LEN)
				{
					strcpy(pacf->WeekContent[i], vstring);
					dprif("WeekContent[%d] : %s\n", i, pacf->WeekContent[i]);
				}
				else
				{
					dprif("WeekContent[%d] critical error\n", i);
				}
			}
		}
		else if (!strcmp(key, "timeFontWidth"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->TimeFontWidth = vint;
				dprif("TimeFontWidth : %d\n", pacf->TimeFontWidth);
			}
			else
			{
				dprif("TimeFontWidth error,current value %d\n", pacf->TimeFontWidth);
			}
		}
		else if (!strcmp(key, "timeFontHeight"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->TimeFontHeight = vint;
				dprif("TimeFontHeight : %d\n", pacf->TimeFontHeight);
			}
			else
			{
				dprif("TimeFontHeight error,current value %d\n", pacf->TimeFontHeight);
			}
		}
		else if (!strcmp(key, "timePictureWidth"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->TimePictureWidth = ((vint+15)/16) * 16;
				dprif("TimePictureWidth : %d\n", pacf->TimePictureWidth);
			}
			else
			{
				dprif("TimePictureWidth error,current value %d\n", pacf->TimePictureWidth);
			}
		}
		else if (!strcmp(key, "timePictureHeight"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->TimePictureHeight = vint;
				dprif("TimePictureHeight : %d\n", pacf->TimePictureHeight);
			}
			else
			{
				dprif("TimePictureHeight error,current value %d\n", pacf->TimePictureHeight);
			}
		}
		else if (!strcmp(key, "timeHourX"))
		{
			vint = json_object_get_int(val);
			pacf->TimeHourX = vint;
			dprif("TimeHourX : %d\n", pacf->TimeHourX);
		}
		else if (!strcmp(key, "timeHourY"))
		{
			vint = json_object_get_int(val);
			pacf->TimeHourY = vint;
			dprif("TimeHourY : %d\n", pacf->TimeHourY);
		}
		else if (!strcmp(key, "timeMinX"))
		{
			vint = json_object_get_int(val);
			pacf->TimeMinX = vint;
			dprif("TimeMinX : %d\n", pacf->TimeMinX);
		}
		else if (!strcmp(key, "timeMinY"))
		{
			vint = json_object_get_int(val);
			pacf->TimeMinY = vint;
			dprif("TimeMinY : %d\n", pacf->TimeMinY);
		}
		else if (!strcmp(key, "timeSecX"))
		{
			vint = json_object_get_int(val);
			pacf->TimeSecX = vint;
			dprif("TimeSecX : %d\n", pacf->TimeSecX);
		}
		else if (!strcmp(key, "timeSecY"))
		{
			vint = json_object_get_int(val);
			pacf->TimeSecY = vint;
			dprif("TimeSecY : %d\n", pacf->TimeSecY);
		}
		else if (!strcmp(key, "timeContentArray"))
		{
			int i;
			int arraylen = json_object_array_length((json_object *)val);

			if (arraylen != 13)
			{
				arraylen = 0;
				dprif("TimeContent array number error set to 0\n");
			}

			dprif("TimeContent arraylen %d\n", arraylen);
			for (i = 0; i < arraylen; i++)
			{
				strcpy(vstring, json_object_get_string(json_object_array_get_idx(val, i)));
				if (strlen(vstring) < PATH_LEN)
				{
					strcpy(pacf->TimeContent[i], vstring);
					dprif("TimeContent[%d] : %s\n", i, pacf->TimeContent[i]);
				}
				else
				{
					dprif("TimeContent[%d] critical error\n", i);
				}
			}
		}
		else
		{
			dprif("ACFTimeConfig key \"%s\" can not recognized\n", key);
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
int CreatFile_ACFTime(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record)
{
	int i;
	uint32 cnt;
	uint32 modelcnt;
	FILE *fp = NULL;
	FileHeader_t header;
	uint32 wcnt = 0;

	dprif("CreatFile_ACFTime\n");

	if ((path == NULL) || (area == NULL))
	{
		dprif("CreatFile_ACFTime parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ACFTime path error\n");
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
	ACFTime_t *acf = area->Ptr;
	uint32 specific_para_addr;
	//uint32 picture_para_addr;

	wcnt += sizeof(FileHeader_t) + sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
	specific_para_addr = wcnt;

	wcnt += sizeof(ACFTime_t) - ACF_TIME_ADDED_LENGTH;
	//picture_para_addr = wcnt;
	area->PictureAddr = wcnt;
	FwritePicture(fp, area->PictureAddr, acf->BackgroundContent, karma, &cnt);

	wcnt += cnt;
	acf->DateContentOffset = wcnt;
	modelcnt = 0;
	for (i = 0; i < 10; i++)
	{
		FwritePicture(fp, acf->DateContentOffset + modelcnt, acf->DateContent[i], karma, &cnt);
		modelcnt += cnt;
	}

	wcnt += modelcnt;
	acf->WeekContentOffset = wcnt;
	modelcnt = 0;
	for (i = 0; i < 7; i++)
	{
		FwritePicture(fp, acf->WeekContentOffset + modelcnt, acf->WeekContent[i], karma, &cnt);
		modelcnt += cnt;
	}

	wcnt += modelcnt;
	acf->TimeContentOffset = wcnt;
	modelcnt = 0;
	for (i = 0; i < 13; i++)
	{
		FwritePicture(fp, acf->TimeContentOffset + modelcnt, acf->TimeContent[i], karma, &cnt);
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
	cnt = fwrite(acf, sizeof(ACFTime_t) - ACF_TIME_ADDED_LENGTH, 1, fp);

	header.Version = FILE_VERSION;
	header.Length = wcnt;
	header.Reserved = 0;
	fseek(fp, 0, SEEK_SET);
	cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

	fclose(fp);

	dprif("CreatFile_ACFTime OK!\n");

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
int FreeMemory_ACFTime(Area_t *area, Record_t *record)
{
	//ACFTime_t *acf;

	if (area == NULL)
	{
		dprif("FreeMemory_ACFTime parameter is NULL\n");
		return -1;
	}

	if (area->Type != AREA_TIME)
	{
		dprif("FreeMemory_ACFTime type is error\n");
		return -2;
	}

	if (area->Ptr != NULL)
	{
		free(area->Ptr);
		area->Ptr = NULL;
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
#define DATA_TYPE           1
#define WEEK_TYPE           2
#define TIME_TYPE           3

//
#define DATE_STYLE_ONE      1               //YYYY/MM/DD
#define DATE_STYLE_TWO      2               //MM/DD/YYYY
#define DATE_STYLE_THREE    3               //DD/MM/YYYY
#define DATE_STYLE_FOUR     4               //YYYYMMDD

//
#define WEEK_STYLE_CH       5               //
#define WEEK_STYLE_EN       6               //3

//
#define TIME_STYLE_ONE      7               //HH:MM:SS
#define TIME_STYLE_TWO      8               //HH:MM
#define TIME_STYLE_THREE    9               //HHMMSS
#define TIME_STYLE_FOUR     10              //HHMM

#define ACF_NEWTIME_ADDED_LENGTH           ((128 + 15) * PATH_LEN)

typedef struct _DigClockPara_
{
	uint8 FontWidth;
	uint8 FontHeight;
	uint8 PictureWidth;    //
	uint8 PictureHeight;
	uint8 Color;
}DigClockPara_t;

typedef struct _NewDigClock_
{
	uint8 JetLag[4];
	uint16 BMask;
	uint8 DateStyle;
	uint8 WeekStyle;
	uint8 TimeStyle;
	uint8 TimeMode;
	uint16 LineDistance;

	uint8 ShowDate;
	uint8 ShowWeek;
	uint8 ShowTime;
	uint16 DateLength;
	uint16 WeekLength;
	uint16 TimeLength;

	DigClockPara_t DateArray[14];
	DigClockPara_t WeekArray[61];
	DigClockPara_t TimeArray[17];

	char DateArrayContent[14][PATH_LEN];
	char WeekArrayContent[61][PATH_LEN];
	char TimeArrayContent[17][PATH_LEN];
}NewDigClock_t;

int CalculateDataLength(uint8 datastyle,DigClockPara_t *datepara);
int CalculateWeekLength(uint8 weekstyle,DigClockPara_t *weekpara);
int CalculateTimeLength(uint8 timestyle,DigClockPara_t *timepara);
uint16 SeekMaxLength(uint16 x,uint16 y,uint16 z);
int CalculateACFTimeCoord(const Coord_t *coord, const WaterEdging_t *wateredge, NewDigClock_t *digpara, ACFTime_t *acf);

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_ACFTimeConfig2(const json_object *new_obj, const Coord_t *coord, const WaterEdging_t *wateredge, ACFTime_t **acf, Record_t *record)
{
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;
	ACFTime_t *pacf;
	NewDigClock_t digclock;

	*acf = malloc(sizeof(ACFTime_t));
	memset(*acf, 0, sizeof(ACFTime_t));
	memset(&digclock,0,sizeof(NewDigClock_t));

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
					digclock.JetLag[0] = 1;
					digclock.JetLag[1] = hourtmp;
				}
				else
				{
					digclock.JetLag[0] = 0;
					digclock.JetLag[1] = abs(hourtmp);
				}
			}
			ptmp = strtok(NULL, ":");//
			if (ptmp != NULL) digclock.JetLag[2] = (uint16)atoi((char*)ptmp);
			ptmp =strtok(NULL, "\0");//
			if (ptmp != NULL) digclock.JetLag[3] = (uint16)atoi((char*)ptmp);
		}
		else if (!strcmp(key, "showDate"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
			{
				digclock.ShowDate = 1;
			}
			else
			{
				digclock.ShowDate = 0;
				digclock.BMask &= ~ACF_TIME_SHOW_YEARH;
				digclock.BMask &= ~ACF_TIME_SHOW_YEARL;
				digclock.BMask &= ~ACF_TIME_SHOW_MONTH;
				digclock.BMask &= ~ACF_TIME_SHOW_DAY;
			}

			dprif("BMask(showDate) : %.4X\n", digclock.BMask);
		}
		else if (!strcmp(key, "showWeek"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
			{
				digclock.ShowWeek = 1;
				digclock.BMask |= ACF_TIME_SHOW_WEEK;
			}
			else
			{
				digclock.ShowWeek = 0;
				digclock.BMask &= ~ACF_TIME_SHOW_WEEK;
			}

			dprif("BMask(showWeek) : %.4X\n", digclock.BMask);
		}
		else if (!strcmp(key, "showTime"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
			{
				digclock.ShowTime = 1;
			}
			else
			{
				digclock.ShowTime = 0;
				digclock.BMask &= ~ACF_TIME_SHOW_HOUR;
				digclock.BMask &= ~ACF_TIME_SHOW_MIN;
				digclock.BMask &= ~ACF_TIME_SHOW_SEC;
			}

			dprif("BMask(showTime) : %.4X\n", digclock.BMask);
		}
		else if (!strcmp(key, "showAPM"))
		{
			vbool = json_object_get_boolean(val);
			if (vbool == TRUE)
			{
				digclock.BMask |= ACF_TIME_SHOW_APM;
			}
			else
			{
				digclock.BMask &= ~ACF_TIME_SHOW_APM;
			}
			dprif("BMask(showAPM) : %.4X\n", digclock.BMask);
		}
		else if (!strcmp(key, "dateStyle"))
		{
			strcpy(vstring, json_object_get_string(val));
			if(!strcmp(vstring, "YYYY/MM/DD"))
			{
				digclock.DateStyle = DATE_STYLE_ONE;
				digclock.BMask |= ACF_TIME_SHOW_YEARH;
				digclock.BMask |= ACF_TIME_SHOW_YEARL;
				digclock.BMask |= ACF_TIME_SHOW_MONTH;
				digclock.BMask |= ACF_TIME_SHOW_DAY;
			}
			else if(!strcmp(vstring, "MM/DD/YYYY"))
			{
				digclock.DateStyle = DATE_STYLE_TWO;
				digclock.BMask |= ACF_TIME_SHOW_YEARH;
				digclock.BMask |= ACF_TIME_SHOW_YEARL;
				digclock.BMask |= ACF_TIME_SHOW_MONTH;
				digclock.BMask |= ACF_TIME_SHOW_DAY;
			}
			else if(!strcmp(vstring, "DD/MM/YYYY"))
			{
				digclock.DateStyle = DATE_STYLE_THREE;
				digclock.BMask |= ACF_TIME_SHOW_YEARH;
				digclock.BMask |= ACF_TIME_SHOW_YEARL;
				digclock.BMask |= ACF_TIME_SHOW_MONTH;
				digclock.BMask |= ACF_TIME_SHOW_DAY;
			}
			else if(!strcmp(vstring, "YYYY#MM#DD#"))
			{
				digclock.DateStyle = DATE_STYLE_FOUR;
				digclock.BMask |= ACF_TIME_SHOW_YEARH;
				digclock.BMask |= ACF_TIME_SHOW_YEARL;
				digclock.BMask |= ACF_TIME_SHOW_MONTH;
				digclock.BMask |= ACF_TIME_SHOW_DAY;
			}
			else
			{
				dprif("dateStyle key error\n");
			}

			dprif("DateStyle : %d\n", digclock.DateStyle);
		}
		else if (!strcmp(key, "weekStyle"))
		{
			strcpy(vstring, json_object_get_string(val));
			if(!strcmp(vstring, "chinese"))
				digclock.WeekStyle = WEEK_STYLE_CH;
			else if(!strcmp(vstring, "english"))
				digclock.WeekStyle = WEEK_STYLE_EN;
			else
			{
				dprif("weekStyle key error\n");
			}

			dprif("WeekStyle : %d\n", digclock.WeekStyle);
		}
		else if (!strcmp(key, "timeStyle"))
		{
			strcpy(vstring, json_object_get_string(val));
			if(!strcmp(vstring, "HH:MM:SS"))
			{
				digclock.TimeStyle = TIME_STYLE_ONE;
				digclock.BMask |= ACF_TIME_SHOW_HOUR;
				digclock.BMask |= ACF_TIME_SHOW_MIN;
				digclock.BMask |= ACF_TIME_SHOW_SEC;
			}
			else if(!strcmp(vstring, "HH:MM"))
			{
				digclock.TimeStyle = TIME_STYLE_TWO;
				digclock.BMask |= ACF_TIME_SHOW_HOUR;
				digclock.BMask |= ACF_TIME_SHOW_MIN;
				digclock.BMask &= ~ACF_TIME_SHOW_SEC;
			}
			else if(!strcmp(vstring, "HH#MM#SS#"))
			{
				digclock.TimeStyle = TIME_STYLE_THREE;
				digclock.BMask |= ACF_TIME_SHOW_HOUR;
				digclock.BMask |= ACF_TIME_SHOW_MIN;
				digclock.BMask |= ACF_TIME_SHOW_SEC;
			}
			else if(!strcmp(vstring, "HH#MM#"))
			{
				digclock.TimeStyle = TIME_STYLE_FOUR;
				digclock.BMask |= ACF_TIME_SHOW_HOUR;
				digclock.BMask |= ACF_TIME_SHOW_MIN;
				digclock.BMask &= ~ACF_TIME_SHOW_SEC;
			}
			else
			{
				dprif("timeStyle key error\n");
			}

			dprif("TimeStyle : %d\n", digclock.TimeStyle);
		}
		else if (!strcmp(key, "timeMode"))
		{
			strcpy(vstring, json_object_get_string(val));
			if(!strcmp(vstring, "24Hours"))
			{
				digclock.BMask |= ACF_TIME_24HOURS_MODE;
			}
			else if(!strcmp(vstring, "12Hours"))
			{
				digclock.BMask &= ~ACF_TIME_24HOURS_MODE;
			}
			else
			{
				dprif("BMask(timeMode) key error\n");
			}

			dprif("BMask(timeMode) : %.4X\n", digclock.BMask);
		}
		else if (!strcmp(key, "lineDistance"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				digclock.LineDistance= vint;
				dprif("LineDistance : %d\n", digclock.LineDistance);
			}
			else
			{
				dprif("LineDistance error,current value %d\n", digclock.LineDistance);
			}
		}
		else if (!strcmp(key, "dateArray"))
		{
			int i;
			int arraylen = json_object_array_length((json_object *)val);

			if (arraylen != 14)
			{
				arraylen = 0;
				dprif("dateArray number error set to 0\n");
			}

			dprif("dateArray arraylen %d\n", arraylen);
			for (i = 0; i < arraylen; i++)
			{
				json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
				{
					if (!strcmp(key_1, "fontWidth"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.DateArray[i].FontWidth = vint;
							dprif("DateArray[%d].FontWidth : %d\n", i, digclock.DateArray[i].FontWidth);
						}
						else
						{
							dprif("DateArray[%d].FontWidth error,current value %d\n", i, digclock.DateArray[i].FontWidth);
						}
					}
					else if (!strcmp(key_1, "fontHeight"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.DateArray[i].FontHeight = vint;
							dprif("DateArray[%d].FontHeight : %d\n", i, digclock.DateArray[i].FontHeight);
						}
						else
						{
							dprif("DateArray[%d].FontHeight error,current value %d\n", i, digclock.DateArray[i].FontHeight);
						}
					}
					else if (!strcmp(key_1, "pictureWidth"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.DateArray[i].PictureWidth = vint;
							dprif("DateArray[%d].PictureWidth : %d\n", i, digclock.DateArray[i].PictureWidth);
						}
						else
						{
							dprif("DateArray[%d].PictureWidth error,current value %d\n", i, digclock.DateArray[i].PictureWidth);
						}
					}
					else if (!strcmp(key_1, "pictureHeight"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.DateArray[i].PictureHeight = vint;
							dprif("DateArray[%d].PictureHeight : %d\n", i, digclock.DateArray[i].PictureHeight);
						}
						else
						{
							dprif("DateArray[%d].PictureHeight error,current value %d\n", i, digclock.DateArray[i].PictureHeight);
						}
					}
					else if (!strcmp(key_1, "color"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.DateArray[i].Color = vint;
							dprif("digclock.DateArray[%d].Color : %d\n", i, digclock.DateArray[i].Color);
						}
						else
						{
							dprif("digclock.DateArray[%d].Color error,current value %d\n", i, digclock.DateArray[i].Color);
						}
					}
					else if (!strcmp(key_1, "content"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(digclock.DateArrayContent[i], vstring);
							dprif("digclock.DateArrayContent[%d] : %s\n", i, digclock.DateArrayContent[i]);
						}
						else
						{
							dprif("digclock.DateArrayContent[%d] critical error\n", i);
						}
					}
				}
			}
		}
		else if (!strcmp(key, "weekArray"))
		{
			int i;
			int arraylen = json_object_array_length((json_object *)val);

			if (arraylen != 61)
			{
				arraylen = 0;
				dprif("weekArray number error set to 0\n");
			}

			dprif("weekArray arraylen %d\n", arraylen);
			for (i = 0; i < arraylen; i++)
			{
				json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
				{
					if (!strcmp(key_1, "fontWidth"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.WeekArray[i].FontWidth = vint;
							dprif("WeekArray[%d].FontWidth : %d\n", i, digclock.WeekArray[i].FontWidth);
						}
						else
						{
							dprif("WeekArray[%d].FontWidth error,current value %d\n", i, digclock.WeekArray[i].FontWidth);
						}
					}
					else if (!strcmp(key_1, "fontHeight"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.WeekArray[i].FontHeight = vint;
							dprif("WeekArray[%d].FontHeight : %d\n", i, digclock.WeekArray[i].FontHeight);
						}
						else
						{
							dprif("WeekArray[%d].FontHeight error,current value %d\n", i, digclock.WeekArray[i].FontHeight);
						}
					}
					else if (!strcmp(key_1, "pictureWidth"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.WeekArray[i].PictureWidth = vint;
							dprif("WeekArray[%d].PictureWidth : %d\n", i, digclock.WeekArray[i].PictureWidth);
						}
						else
						{
							dprif("WeekArray[%d].PictureWidth error,current value %d\n", i, digclock.WeekArray[i].PictureWidth);
						}
					}
					else if (!strcmp(key_1, "pictureHeight"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.WeekArray[i].PictureHeight = vint;
							dprif("WeekArray[%d].PictureHeight : %d\n", i, digclock.WeekArray[i].PictureHeight);
						}
						else
						{
							dprif("WeekArray[%d].PictureHeight error,current value %d\n", i, digclock.WeekArray[i].PictureHeight);
						}
					}
					else if (!strcmp(key_1, "color"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.WeekArray[i].Color = vint;
							dprif("digclock.WeekArray[%d].Color : %d\n", i, digclock.WeekArray[i].Color);
						}
						else
						{
							dprif("digclock.WeekArray[%d].Color error,current value %d\n", i, digclock.WeekArray[i].Color);
						}
					}
					else if (!strcmp(key_1, "content"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(digclock.WeekArrayContent[i], vstring);
							dprif("digclock.WeekArrayContent[%d] : %s\n", i, digclock.WeekArrayContent[i]);
						}
						else
						{
							dprif("digclock.WeekArrayContent[%d] critical error\n", i);
						}
					}
				}
			}
		}
		else if (!strcmp(key, "timeArray"))
		{
			int i;
			int arraylen = json_object_array_length((json_object *)val);

			if (arraylen != 17)
			{
				arraylen = 0;
				dprif("timeArray number error set to 0\n");
			}

			dprif("timeArray arraylen %d\n", arraylen);
			for (i = 0; i < arraylen; i++)
			{
				json_object_object_foreach(json_object_array_get_idx(val, i), key_1, val_1)
				{
					if (!strcmp(key_1, "fontWidth"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.TimeArray[i].FontWidth = vint;
							dprif("TimeArray[%d].FontWidth : %d\n", i, digclock.TimeArray[i].FontWidth);
						}
						else
						{
							dprif("TimeArray[%d].FontWidth error,current value %d\n", i, digclock.TimeArray[i].FontWidth);
						}
					}
					else if (!strcmp(key_1, "fontHeight"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.TimeArray[i].FontHeight = vint;
							dprif("TimeArray[%d].FontHeight : %d\n", i, digclock.TimeArray[i].FontHeight);
						}
						else
						{
							dprif("TimeArray[%d].FontHeight error,current value %d\n", i, digclock.TimeArray[i].FontHeight);
						}
					}
					else if (!strcmp(key_1, "pictureWidth"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.TimeArray[i].PictureWidth = vint;
							dprif("TimeArray[%d].PictureWidth : %d\n", i, digclock.TimeArray[i].PictureWidth);
						}
						else
						{
							dprif("TimeArray[%d].PictureWidth error,current value %d\n", i, digclock.TimeArray[i].PictureWidth);
						}
					}
					else if (!strcmp(key_1, "pictureHeight"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.TimeArray[i].PictureHeight = vint;
							dprif("TimeArray[%d].PictureHeight : %d\n", i, digclock.TimeArray[i].PictureHeight);
						}
						else
						{
							dprif("TimeArray[%d].PictureHeight error,current value %d\n", i, digclock.TimeArray[i].PictureHeight);
						}
					}
					else if (!strcmp(key_1, "color"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							digclock.TimeArray[i].Color = vint;
							dprif("digclock.TimeArray[%d].Color : %d\n", i, digclock.TimeArray[i].Color);
						}
						else
						{
							dprif("digclock.TimeArray[%d].Color error,current value %d\n", i, digclock.TimeArray[i].Color);
						}
					}
					else if (!strcmp(key_1, "content"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(digclock.TimeArrayContent[i], vstring);
							dprif("digclock.TimeArrayContent[%d] : %s\n", i, digclock.TimeArrayContent[i]);
						}
						else
						{
							dprif("digclock.TimeArrayContent[%d] critical error\n", i);
						}
					}
				}
			}
		}
		else
		{
			dprif("NewTimeConfig key \"%s\" can not recognized\n", key);
		}
	}

	CalculateACFTimeCoord(coord, wateredge, &digclock, pacf);

	return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CalculateACFTimeCoord(const Coord_t *coord, const WaterEdging_t *wateredge, NewDigClock_t *digpara, ACFTime_t *acf)
{
	uint16 linetotal = 0;
	uint16 linenum = 0;
	uint16 datalength = 0;
	uint16 weeklength = 0;
	uint16 timelength = 0;
	uint16 maxwidth = 0;
	uint16 maxheight = 0;

	int16 coordrealwidth = 0;
	int16 coordrealheight = 0;
	int16 sourcex = 0;
	int16 sourcey = 0;
	int16 dstx = 0;
	int16 dsty = 0;
	int i = 0;

	char dir[PATH_LEN];
	char filename[NAME_LEN];
	memset(dir,0,PATH_LEN);
	memset(filename,0,NAME_LEN);
	if(wateredge->Mode & WATER_EDGING_MODE_ENABLE)
	{
		coordrealwidth = coord->Width - wateredge->Height*2;
		coordrealheight = coord->Height - wateredge->Height*2;
	}
	else
	{
		coordrealwidth = coord->Width;
		coordrealheight = coord->Height;
	}

	if (coordrealwidth < 0 || coordrealwidth < 0)
	{
		dprif("coord width or height wrong\n");
		return -1;
	}

	acf->StartX = 0;
	acf->StartY = 0;
	acf->BMask = digpara->BMask;
	for(i = 0;i < 4;i++)
	{
		acf->JetLag[i] = digpara->JetLag[i];
	}
	//weekacf
	for(i = 0;i < 7;i++)
	{
		memcpy(acf->WeekContent[i],digpara->WeekArrayContent[i + 2],strlen(digpara->WeekArrayContent[i + 2]));
	}
	//0123456789/
	acf->DateFontWidth = digpara->DateArray[3].FontWidth;
	acf->DateFontHeight = digpara->DateArray[3].FontHeight;
	acf->DatePictureWidth = (acf->DateFontWidth + 15)/16 * 16;
	acf->DatePictureHeight = digpara->DateArray[3].PictureHeight;

	//ABC~XYZabc~xyz
	acf->WeekFontWidth = digpara->WeekArray[2].FontWidth;
	acf->WeekFontHeight = digpara->WeekArray[2].FontHeight;
	acf->WeekPictureWidth = (acf->WeekFontWidth + 15)/16 * 16;
	acf->WeekPictureHeight = digpara->DateArray[2].PictureHeight;

	//0123456789:
	acf->TimeFontWidth = digpara->TimeArray[3].FontWidth;
	acf->TimeFontHeight = digpara->TimeArray[3].FontHeight;
	acf->TimePictureWidth = (acf->TimeFontWidth + 15)/16 * 16;
	acf->TimePictureHeight = digpara->DateArray[3].PictureHeight;

	//
	uint8 tmpheight = acf->DateFontHeight;

	uint8 tmpdatewidth = acf->DateFontWidth;
	uint8 tmpweekwidth = acf->WeekFontWidth;
	uint8 tmptimewidth = acf->TimeFontWidth;


	//
	if((0 == digpara->DateArrayContent[0][0])||(0 == digpara->WeekArrayContent[0][0])||(0 == digpara->TimeArrayContent[0][0]))
	{
		dprif("bg.bmp file path error\n");
		return -1;
	}
	DetachFilePath(digpara->DateArrayContent[0], dir, filename);

	dprif(" filename %s\n",digpara->DateArrayContent[0]);
	memcpy(&filename[strlen(filename)-4],"_bg.bmp",strlen("_bg.bmp"));
	sprintf(dir, "%s%s", dir, filename);
	CreateBMPfile(dir, coordrealwidth, coordrealheight, BIT_DEPTH, 0);
	dprif("coord->Width %d, coord->Height %d\n",coord->Width, coord->Height);

	//
	linetotal = digpara->ShowDate + digpara->ShowWeek + digpara->ShowTime;
	datalength = CalculateDataLength(digpara->DateStyle, digpara->DateArray);
	weeklength = CalculateWeekLength(digpara->WeekStyle, digpara->WeekArray);
	timelength = CalculateTimeLength(digpara->TimeStyle, digpara->TimeArray);
	dprif("datalength %d,weeklength %d ,timelength %d\n",datalength,weeklength,timelength);

	maxwidth = SeekMaxLength(datalength,weeklength,timelength);
	maxheight = acf->DatePictureHeight * linetotal + (linetotal - 1) * digpara->LineDistance;
	if(maxheight < coordrealheight)
	{
		sourcey = (coordrealheight - maxheight) / 2;
	}
	else
	{
		sourcey = 0;
	}
	if(maxwidth < coordrealwidth)
	{
		sourcex = (coordrealwidth - maxwidth) / 2;
	}
	else
	{
		sourcex = 0;
	}
	dprif("sourcex %d,sourcey %d\n",sourcex,sourcey);

	//
	if(1 == digpara->ShowDate)
	{
		linenum++;
		dsty = sourcey + tmpheight * (linenum-1)  + (linenum - 1) * digpara->LineDistance;
		acf->DateYearHY = dsty;
		acf->DateYearLY = dsty;
		acf->DateMonthY = dsty;
		acf->DateDayY = dsty;

		dstx = sourcex + (maxwidth - datalength) / 2;
		dprif("linenum %d,dstx %d,dsty %d\n",linenum,dstx,dsty);
		if(digpara->DateStyle == DATE_STYLE_ONE)   //YYYY/MM/DD
		{
			acf->DateYearHX = dstx;
			acf->DateYearLX = acf->DateYearHX + 2 * acf->DateFontWidth;
			acf->DateMonthX = acf->DateYearLX + 2 * acf->DateFontWidth + digpara->DateArray[13].FontWidth;
			acf->DateDayX = acf->DateMonthX + 2 * acf->DateFontWidth + digpara->DateArray[13].FontWidth;
			dstx = acf->DateYearHX + 4 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[13],dstx,dsty,0, 0,digpara->DateArray[13].FontWidth,tmpheight);
			dstx = acf->DateMonthX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[13],dstx,dsty,0, 0,digpara->DateArray[13].FontWidth,tmpheight);
		}
		else if(digpara->DateStyle == DATE_STYLE_TWO)    //MM/DD/YYYY
		{
			acf->DateMonthX = dstx;
			acf->DateDayX = acf->DateMonthX + 2 * acf->DateFontWidth + digpara->DateArray[13].FontWidth;
			acf->DateYearHX = acf->DateDayX + 2 * acf->DateFontWidth + digpara->DateArray[13].FontWidth;
			acf->DateYearLX = acf->DateYearHX + 2 * acf->DateFontWidth;
			dstx = acf->DateMonthX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[13],dstx,dsty,0, 0,digpara->DateArray[13].FontWidth,tmpheight);
			dstx = acf->DateDayX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[13],dstx,dsty,0, 0,digpara->DateArray[13].FontWidth,tmpheight);
		}
		else if(digpara->DateStyle == DATE_STYLE_THREE)  //DD/MM/YYYY
		{
			acf->DateDayX = dstx;
			acf->DateMonthX = acf->DateDayX + 2 * acf->DateFontWidth + digpara->DateArray[13].FontWidth;
			acf->DateYearHX = acf->DateMonthX + 2 * acf->DateFontWidth + digpara->DateArray[13].FontWidth;
			acf->DateYearLX = acf->DateYearHX + 2 * acf->DateFontWidth;
			dstx = acf->DateDayX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[13],dstx,dsty,0, 0,digpara->DateArray[13].FontWidth,tmpheight);
			dstx = acf->DateMonthX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[13],dstx,dsty,0, 0,digpara->DateArray[13].FontWidth,tmpheight);
		}
		else if(digpara->DateStyle == DATE_STYLE_FOUR)    //YYYYMMDD
		{
			acf->DateYearHX = dstx;
			acf->DateYearLX = acf->DateYearHX + 2 * acf->DateFontWidth;
			acf->DateMonthX = acf->DateYearLX + 2 * acf->DateFontWidth + digpara->DateArray[0].FontWidth;
			acf->DateDayX = acf->DateMonthX + 2 * acf->DateFontWidth + digpara->DateArray[1].FontWidth;
			dstx = acf->DateYearHX + 4 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[0],dstx,dsty,0, 0,digpara->DateArray[0].FontWidth,tmpheight);
			dstx = acf->DateMonthX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[1],dstx,dsty,0, 0,digpara->DateArray[1].FontWidth,tmpheight);
			dstx = acf->DateDayX + 2 * acf->DateFontWidth;
			PasteMaterialBMP(dir,digpara->DateArrayContent[2],dstx,dsty,0, 0,digpara->DateArray[2].FontWidth,tmpheight);
		}
		else
		{
			dprif("DateStyle is not exist\n");
			return -1;
		}
	}

	if(1 == digpara->ShowWeek)
	{
		linenum++;
		dsty = sourcey + tmpheight * (linenum-1)  + (linenum - 1) * digpara->LineDistance;
		acf->WeekY = dsty;

		dstx = sourcex + (maxwidth - weeklength) / 2;
		dprif("linenum %d,dstx %d,dsty %d\n",linenum,dstx,dsty);
		if(digpara->WeekStyle == WEEK_STYLE_CH)//
		{
			acf->WeekX = dstx + digpara->WeekArray[0].FontWidth + digpara->WeekArray[1].FontWidth;
			PasteMaterialBMP(dir,digpara->WeekArrayContent[0],dstx,dsty,0, 0,digpara->WeekArray[0].FontWidth,tmpheight);
			dstx = dstx + digpara->WeekArray[0].FontWidth;
			PasteMaterialBMP(dir,digpara->WeekArrayContent[1],dstx,dsty,0, 0,digpara->WeekArray[1].FontWidth,tmpheight);
		}
		else if(digpara->WeekStyle == WEEK_STYLE_EN)//Sun Mon
		{
			uint16 week[7][3] = {{27,55,48},{21,49,48},{28,55,39},{31,39,38},{28,42,55},{14,52,43},{27,35,54}};
			char weekbmpdir[PATH_LEN];
			char weekbmpfile[NAME_LEN];
			memset(weekbmpdir,0,PATH_LEN);
			memset(weekbmpfile,0,NAME_LEN);
			acf->WeekX = dstx;
			acf->WeekFontWidth = digpara->WeekArray[9].FontWidth + 2 * digpara->WeekArray[35].FontWidth;
			acf->WeekPictureWidth = (acf->WeekFontWidth + 15)/16 * 16;

			for(i = 0;i < 7;i++)
			{
				DetachFilePath(digpara->WeekArrayContent[i], weekbmpdir, weekbmpfile);
				memcpy(&weekbmpfile[strlen(weekbmpfile)-4],"_Eng.bmp",strlen("_Eng.bmp"));
				sprintf(weekbmpdir, "%s%s", weekbmpdir, weekbmpfile);
				CreateBMPfile(weekbmpdir, acf->WeekFontWidth, acf->WeekFontHeight, BIT_DEPTH, 0);
				//weekacf
				memcpy(acf->WeekContent[i],weekbmpdir,strlen(weekbmpdir));
				dprif("weekenglish %s\n",weekbmpdir);
				dprif("%d\n",digpara->WeekArray[35].FontWidth);
				dprif("%s\n",digpara->WeekArrayContent[week[i][0]]);
				PasteMaterialBMP(weekbmpdir,digpara->WeekArrayContent[week[i][0]],0,0,0,0,digpara->WeekArray[9].FontWidth,tmpheight);
				PasteMaterialBMP(weekbmpdir,digpara->WeekArrayContent[week[i][1]],digpara->WeekArray[9].FontWidth,0,0,0,digpara->WeekArray[35].FontWidth,tmpheight);
				PasteMaterialBMP(weekbmpdir,digpara->WeekArrayContent[week[i][2]],digpara->WeekArray[9].FontWidth + digpara->WeekArray[35].FontWidth,0,0,0,digpara->WeekArray[35].FontWidth,tmpheight);
			}
		}
		else
		{
			dprif("WeekStyle is not exist\n");
			return -1;
		}
	}

	if(1 == digpara->ShowTime)
	{
		linenum++;
		dsty = sourcey + tmpheight * (linenum-1)  + (linenum - 1) * digpara->LineDistance;
		acf->TimeHourY = dsty;
		acf->TimeMinY = dsty;
		acf->TimeSecY = dsty;

		dstx = sourcex + (maxwidth - timelength) / 2;
		dprif("linenum %d,dstx %d,dsty %d\n",linenum,dstx,dsty);
		if(digpara->TimeStyle == TIME_STYLE_ONE)//HH:MM:SS
		{
			acf->TimeHourX = dstx;
			acf->TimeMinX = acf->TimeHourX + 2 * acf->TimeFontWidth + digpara->TimeArray[16].FontWidth;
			acf->TimeSecX = acf->TimeMinX + 2 * acf->TimeFontWidth + digpara->TimeArray[16].FontWidth;
			dstx = acf->TimeHourX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[16],dstx,dsty,0, 0,digpara->TimeArray[16].FontWidth,tmpheight);
			dstx = acf->TimeMinX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[16],dstx,dsty,0, 0,digpara->TimeArray[16].FontWidth,tmpheight);
		}
		else if(digpara->TimeStyle == TIME_STYLE_TWO)//HH:MM
		{
			acf->TimeHourX = dstx;
			acf->TimeMinX = acf->TimeHourX + 2 * acf->TimeFontWidth + digpara->TimeArray[16].FontWidth;
			dstx = acf->TimeHourX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[16],dstx,dsty,0, 0,digpara->TimeArray[16].FontWidth,tmpheight);
		}
		else if(digpara->TimeStyle == TIME_STYLE_THREE)//HH#MM#SS#
		{
			acf->TimeHourX = dstx;
			acf->TimeMinX = acf->TimeHourX + 2 * acf->TimeFontWidth + digpara->TimeArray[0].FontWidth;
			acf->TimeSecX = acf->TimeMinX + 2 * acf->TimeFontWidth + digpara->TimeArray[1].FontWidth;
			dstx = acf->TimeHourX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[0],dstx,dsty,0, 0,digpara->TimeArray[0].FontWidth,tmpheight);
			dstx = acf->TimeMinX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[1],dstx,dsty,0, 0,digpara->TimeArray[1].FontWidth,tmpheight);
			dstx = acf->TimeSecX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[2],dstx,dsty,0, 0,digpara->TimeArray[2].FontWidth,tmpheight);
		}
		else if(digpara->TimeStyle == TIME_STYLE_FOUR)//HH#MM#
		{
			acf->TimeHourX = dstx;
			acf->TimeMinX = acf->TimeHourX + 2 * acf->TimeFontWidth + digpara->TimeArray[0].FontWidth;
			dstx = acf->TimeHourX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[0],dstx,dsty,0, 0,digpara->TimeArray[0].FontWidth,tmpheight);
			dstx = acf->TimeMinX + 2 * acf->TimeFontWidth;
			PasteMaterialBMP(dir,digpara->TimeArrayContent[1],dstx,dsty,0, 0,digpara->TimeArray[1].FontWidth,tmpheight);
		}
		else
		{
			dprif("TimeStyle is not exist\n");
			return -1;
		}
	}

	//acf
	memcpy(acf->BackgroundContent,dir,strlen(dir));
	dprif("acf->BackgroundContent: %s\n",acf->BackgroundContent);

	//dateacf
	for(i = 0;i < 10;i++)
	{
		memcpy(acf->DateContent[i],digpara->DateArrayContent[i + 3],strlen(digpara->DateArrayContent[i + 3]));
	}

	//timeacf
	for(i = 0;i < 13;i++)
	{
		memcpy(acf->TimeContent[i],digpara->TimeArrayContent[i + 3],strlen(digpara->TimeArrayContent[i + 3]));
	}

	return 0;
}



int CalculateDataLength(uint8 datastyle,DigClockPara_t *datepara)
{
	switch (datastyle)
	{
		case DATE_STYLE_ONE:
			return (8*datepara[3].FontWidth + 2*datepara[13].FontWidth);
		case DATE_STYLE_TWO:
			return (8*datepara[3].FontWidth + 2*datepara[13].FontWidth);
		case DATE_STYLE_THREE:
			return (8*datepara[3].FontWidth + 2*datepara[13].FontWidth);
		case DATE_STYLE_FOUR:
			return (8*datepara[3].FontWidth + datepara[0].FontWidth + datepara[1].FontWidth + datepara[2].FontWidth);
		default:
			return 0;
	}
}

int CalculateWeekLength(uint8 weekstyle,DigClockPara_t *weekpara)
{
	switch (weekstyle)
	{
		case WEEK_STYLE_CH:
			return (weekpara[0].FontWidth + weekpara[1].FontWidth + weekpara[2].FontWidth);
		case WEEK_STYLE_EN:
			return (weekpara[9].FontWidth + 2*weekpara[35].FontWidth);
		default:
			return 0;
	}
}

int CalculateTimeLength(uint8 timestyle,DigClockPara_t *timepara)
{
	switch (timestyle)
	{
		case TIME_STYLE_ONE:
			return (6*timepara[3].FontWidth + 2*timepara[16].FontWidth);
		case TIME_STYLE_TWO:
			return (4*timepara[3].FontWidth + timepara[16].FontWidth);
		case TIME_STYLE_THREE:
			return (6*timepara[3].FontWidth + timepara[0].FontWidth + timepara[1].FontWidth + timepara[2].FontWidth);
		case TIME_STYLE_FOUR:
			return (4*timepara[3].FontWidth + timepara[0].FontWidth + timepara[1].FontWidth);
		default:
			return 0;
	}
}

uint16 SeekMaxLength(uint16 x,uint16 y,uint16 z)
{
	int temp;
	temp = x>y?x:y;
	return (temp>z?temp:z);
	//return ((x>y?x:y)>z?(x>y?x:y):z)
}

