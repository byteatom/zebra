#include "kaler_config.h"
#include "../json/json.h"
#include "parse_program.h"

#include "parse_acfword.h"
#include "parse_acftime.h"
#include "parse_acfwatch.h"
#include "parse_acfshine.h"
#include "parse_acfcount.h"
#include "parse_acftemperature.h"
#include "parse_acfcolorful.h"

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
int JsonParse_ProgramDescribe(const json_object *new_obj, ProgramDescribe_t *program, Record_t *record)
{
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;

	memset(program, 0, sizeof(ProgramDescribe_t));

	int i;
	int arraylen = json_object_array_length((json_object *)new_obj);

	if (arraylen < 0)
	{
		arraylen = 0;
		dprif("program array number error set to 0");
	}

	program->CCF.ProgramTotal = arraylen;

	program->CCF.ProgramPtr = malloc(sizeof(CCFProgram_t)*arraylen);
	memset(program->CCF.ProgramPtr, 0, sizeof(CCFProgram_t)*arraylen);

	program->BCF = malloc(sizeof(BCF_t)*arraylen);
	memset(program->BCF, 0, sizeof(BCF_t)*arraylen);

	program->PCF = malloc(sizeof(PCF_t)*arraylen);
	memset(program->PCF, 0, sizeof(PCF_t)*arraylen);

	dprif("program total %d\n", program->CCF.ProgramTotal);

	for (i = 0; i < arraylen; i++)
	{
		CCFProgram_t *p = &program->CCF.ProgramPtr[i];

		dprif("\n", i);
		dprif("[----------------- program %d -----------------]\n", i);

		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "name"))
			{
				strcpy(vstring, json_object_get_string(val));
				if ((strlen(vstring) < SHORT_NAME_LEN) && (strcmp(vstring, "none") != 0))
				{
					strcpy(p->ProgramName, vstring);
					strcpy(program->PCF[i].Name, vstring);
					dprif("programName : %s\n", p->ProgramName);
				}
				else
				{
					sprintf(p->ProgramName, "prog_%d.pcf", i);
					sprintf(program->PCF[i].Name, "prog_%d.pcf", i);
					dprif("ProgramName : %s (default set)\n", p->ProgramName);
				}
			}
			else if (!strcmp(key, "playControl"))
			{
				dprif("<<<<<<<<<<<< PlayControl >>>>>>>>>>>>\n");
				json_object_object_foreach(val, key_1, val_1)
				{
					if (!strcmp(key_1, "timeMode"))
					{
						strcpy(vstring, json_object_get_string(val_1));

						if(!strcmp(vstring, "fixLength"))
							p->PlayControl.ControlMode = PROGRAM_DELAY_FIXLENGTH;
						else if(!strcmp(vstring, "fixDate"))
							p->PlayControl.ControlMode = PROGRAM_DELAY_FIXDATE;
						else if(!strcmp(vstring, "external"))
							p->PlayControl.ControlMode = PROGRAM_DELAY_EXTERNAL;
						else if(!strcmp(vstring, "sequence"))
						{
							p->PlayControl.ControlMode = PROGRAM_DELAY_SEQUENCE;
							p->PlayControl.TimeUnion.Timelong = 0;
						}
						else
						{
							dprif("timeMode key error\n");
						}

						dprif("ControlMode : %d\n", p->PlayControl.ControlMode);
					}
					else if (!strcmp(key_1, "timeLong"))
					{
						vint = json_object_get_int(val_1);
						if(p->PlayControl.ControlMode & PROGRAM_DELAY_FIXDATE)
						{
							dprif("Timelong not deal\n");
						}
						else
						{
							if (vint >= 0)
							{
								p->PlayControl.TimeUnion.Timelong = vint;
								dprif("Timelong : %lu\n", p->PlayControl.TimeUnion.Timelong);
							}
							else
							{
								dprif("Timelong error,current value %lu\n", p->PlayControl.TimeUnion.Timelong);
							}
						}
					}
					else if (!strcmp(key_1, "beginTime"))
					{
						JsonParse_DateTime(val_1, &p->PlayControl.TimeUnion.TimeBE.Begintime, PARSE_WEEK_MODE_MASK);
						dprif("begin date : %d-%d-%d\n",
							p->PlayControl.TimeUnion.TimeBE.Begintime.Year,
							p->PlayControl.TimeUnion.TimeBE.Begintime.Month,
							p->PlayControl.TimeUnion.TimeBE.Begintime.Day);
						dprif("begin week : %#X\n",
							p->PlayControl.TimeUnion.TimeBE.Begintime.Week);
						dprif("begin time : %d:%d:%d\n",
							p->PlayControl.TimeUnion.TimeBE.Begintime.Hour,
							p->PlayControl.TimeUnion.TimeBE.Begintime.Min,
							p->PlayControl.TimeUnion.TimeBE.Begintime.Sec);

					}
					else if (!strcmp(key_1, "endTime"))
					{
						JsonParse_DateTime(val_1, &p->PlayControl.TimeUnion.TimeBE.Endtime, PARSE_WEEK_MODE_MASK);
						dprif("end date : %d-%d-%d\n",
							p->PlayControl.TimeUnion.TimeBE.Endtime.Year,
							p->PlayControl.TimeUnion.TimeBE.Endtime.Month,
							p->PlayControl.TimeUnion.TimeBE.Endtime.Day);
						dprif("end week : %#X\n",
							p->PlayControl.TimeUnion.TimeBE.Endtime.Week);
						dprif("end time : %d:%d:%d\n",
							p->PlayControl.TimeUnion.TimeBE.Endtime.Hour,
							p->PlayControl.TimeUnion.TimeBE.Endtime.Min,
							p->PlayControl.TimeUnion.TimeBE.Endtime.Sec);

					}
					else if (!strcmp(key_1, "cmpDate"))
					{
						vbool = json_object_get_boolean(val_1);
						if (vbool == TRUE)
						{
							p->PlayControl.ControlMode |= PROGRAM_COMPARE_DATE;
						}
						else
						{
							p->PlayControl.ControlMode &= ~PROGRAM_COMPARE_DATE;
						}

						dprif("ControlMode : %.4X\n", p->PlayControl.ControlMode);
					}
					else if (!strcmp(key_1, "cmpTime"))
					{
						vbool = json_object_get_boolean(val_1);
						if (vbool == TRUE)
						{
							p->PlayControl.ControlMode |= PROGRAM_COMPARE_TIME;
						}
						else
						{
							p->PlayControl.ControlMode &= ~PROGRAM_COMPARE_TIME;
						}

						dprif("ControlMode : %.4X\n", p->PlayControl.ControlMode);
					}
				}
			}
			else if (!strcmp(key, "shineBackground"))
			{
				dprif("<<<<<<<<<<<< ShineBackground >>>>>>>>>>>>\n");
				json_object_object_foreach(val, key_1, val_1)
				{
					if (!strcmp(key_1, "name"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if ((strlen(vstring) < SHORT_NAME_LEN) && (strcmp(vstring, "none") != 0))
						{
							strcpy(p->ShineBackgroundName, vstring);
							strcpy(program->BCF[i].Name, vstring);
							dprif("ShineBackgroundName : %s\n", p->ShineBackgroundName);
						}
						else
						{
							sprintf(p->ShineBackgroundName, "bkgrd_%d.bcf", i);
							sprintf(program->BCF[i].Name, "bkgrd_%d.bcf", i);
							dprif("ShineBackgroundName : %s (default set)\n", p->ShineBackgroundName);
						}
					}
					else if (!strcmp(key_1, "frameTotal"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->BCF[i].FrameTotal = vint;
							dprif("FrameTotal : %d\n", program->BCF[i].FrameTotal);
						}
						else
						{
							dprif("FrameTotal error,current value %d\n", program->BCF[i].FrameTotal);
						}
					}
					else if (!strcmp(key_1, "pictureTotal"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->BCF[i].PictureTotal = vint;
							dprif("PictureTotal : %d\n", program->BCF[i].PictureTotal);
						}
						else
						{
							dprif("PictureTotal error,current value %d\n", program->BCF[i].PictureTotal);
						}
					}
					else if (!strcmp(key_1, "elementTotal"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->BCF[i].ElementTotal = vint;
							dprif("ElementTotal : %d\n", program->BCF[i].ElementTotal);
						}
						else
						{
							dprif("ElementTotal error,current value %d\n", program->BCF[i].ElementTotal);
						}
					}
					else if (!strcmp(key_1, "frameSpeed"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->BCF[i].FrameSpeed = vint;
							dprif("FrameSpeed : %d\n", program->BCF[i].FrameSpeed);
						}
						else
						{
							dprif("FrameSpeed error,current value %d\n", program->BCF[i].FrameSpeed);
						}
					}
					else if (!strcmp(key_1, "pictureArray"))
					{
						JsonParse_ShinePictureArray(val_1, &program->BCF[i].PicturePtr, &program->BCF[i].PictureTotal);
					}
					else if (!strcmp(key_1, "elementArray"))
					{
						JsonParse_ShineElementArray(val_1, &program->BCF[i].ElementPtr, &program->BCF[i].ElementTotal);
					}
				}
			}
			else if (!strcmp(key, "waterEdging"))
			{
				dprif("<<<<<<<<<<<< WaterEdging >>>>>>>>>>>>\n");
				json_object_object_foreach(val, key_1, val_1)
				{
					if (!strcmp(key_1, "width"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->PCF[i].WaterEdging.Width = vint;
							dprif("Width : %d\n", program->PCF[i].WaterEdging.Width);
						}
						else
						{
							dprif("Width error,current value %d\n", program->PCF[i].WaterEdging.Width);
						}
					}
					else if (!strcmp(key_1, "height"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->PCF[i].WaterEdging.Height = vint;
							dprif("Height : %d\n", program->PCF[i].WaterEdging.Height);
						}
						else
						{
							dprif("Height error,current value %d\n", program->PCF[i].WaterEdging.Height);
						}
					}
					else if (!strcmp(key_1, "speed"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							program->PCF[i].WaterEdging.Speed = vint;
							dprif("Speed : %d\n", program->PCF[i].WaterEdging.Speed);
						}
						else
						{
							dprif("Speed error,current value %d\n", program->PCF[i].WaterEdging.Speed);
						}
					}
					else if (!strcmp(key_1, "flicker"))
					{
						vbool = json_object_get_boolean(val_1);
						if (vbool == TRUE)
						{
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_FLICKER;
						}
						else
						{
							program->PCF[i].WaterEdging.Mode &= ~WATER_EDGING_FLICKER;
						}

						dprif("Mode : %.4X\n", program->PCF[i].WaterEdging.Mode);
					}
					else if (!strcmp(key_1, "mode"))
					{
						strcpy(vstring, json_object_get_string(val_1));

						if(!strcmp(vstring, "forward"))
						{
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_MODE(WE_FORWARD);
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_MODE_ENABLE;
						}
						else if(!strcmp(vstring, "reversal"))
						{
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_MODE(WE_REVERSAL);
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_MODE_ENABLE;
						}
						else if(!strcmp(vstring, "static"))
						{
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_MODE(WE_STATIC);
							program->PCF[i].WaterEdging.Mode |= WATER_EDGING_MODE_ENABLE;
						}
						else if(!strcmp(vstring, "none"))
						{
							program->PCF[i].WaterEdging.Mode &= ~WATER_EDGING_MODE_ENABLE;
						}
						else
						{
							dprif("WaterEdging.Mode key error\n");
						}

						dprif("Mode : %.4X\n", program->PCF[i].WaterEdging.Mode);
					}
					else if (!strcmp(key_1, "pictureUpContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(program->PCF[i].WaterEdging.PictureUpContent, vstring);
							dprif("PictureUpContent : %s\n", program->PCF[i].WaterEdging.PictureUpContent);
						}
						else
						{
							dprif("PictureUpContent critical error\n");
						}
					}
					else if (!strcmp(key_1, "pictureDownContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(program->PCF[i].WaterEdging.PictureDownContent, vstring);
							dprif("PictureDownContent : %s\n", program->PCF[i].WaterEdging.PictureDownContent);
						}
						else
						{
							dprif("PictureDownContent critical error\n");
						}
					}
					else if (!strcmp(key_1, "pictureLeftContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(program->PCF[i].WaterEdging.PictureLeftContent, vstring);
							dprif("PictureLeftContent : %s\n", program->PCF[i].WaterEdging.PictureLeftContent);
						}
						else
						{
							dprif("PictureLeftContent critical error\n");
						}
					}
					else if (!strcmp(key_1, "pictureRightContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(program->PCF[i].WaterEdging.PictureRightContent, vstring);
							dprif("PictureRightContent : %s\n", program->PCF[i].WaterEdging.PictureRightContent);
						}
						else
						{
							dprif("PictureRightContent critical error\n");
						}
					}
				}
			}
			else if (!strcmp(key, "areaArray"))
			{
				char tmp[SHORT_NAME_LEN];
				sprintf(tmp, "p%d", i);

				JsonParse_AreaArray(val,
									&program->PCF[i].Area,
									&program->PCF[i].AreaTotal,
									tmp,
									record);
			}
			else
			{
				dprif("ProgramDescribe key \"%s\" can not recognized\n", key);
			}
		}

		if (p->ShineBackgroundName[0] == 0)
		{
			sprintf(p->ShineBackgroundName, "bkgrd_%d.bcf", i);
			sprintf(program->BCF[i].Name, "bkgrd_%d.bcf", i);
			dprif("ShineBackgroundName : %s (additional set)\n", p->ShineBackgroundName);
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
int JsonParse_ShinePictureArray(const json_object *new_obj, ShinePicture_t **picture, uint16 *number)
{
	ShinePicture_t *ppicture;
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;

	int i;
	int arraylen = json_object_array_length((json_object *)new_obj);

	if (arraylen < 0)
	{
		arraylen = 0;
		dprif("ShinePicture array number error set to 0");
	}

	*number = arraylen;
	*picture = malloc(sizeof(ShinePicture_t)*arraylen);
	memset(*picture, 0, sizeof(ShinePicture_t)*arraylen);

	ppicture = *picture;

	for (i = 0; i < arraylen; i++)
	{
		dprif("* ShinePicture %d\n", i);
		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "width"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					ppicture[i].Width = vint;
					dprif("Width : %d\n", ppicture[i].Width);
				}
				else
				{
					dprif("Width error,current value %d\n", ppicture[i].Width);
				}
			}
			else if (!strcmp(key, "height"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					ppicture[i].Height= vint;
					dprif("Height : %d\n", ppicture[i].Height);
				}
				else
				{
					dprif("Height error,current value %d\n", ppicture[i].Height);
				}
			}
			else if (!strcmp(key, "content"))
			{
				strcpy(vstring, json_object_get_string(val));
				if (strlen(vstring) < PATH_LEN)
				{
					strcpy(ppicture[i].PictureContent, vstring);
					dprif("PictureContent : %s\n", ppicture[i].PictureContent);
				}
				else
				{
					dprif("PictureContent critical error\n");
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
int JsonParse_ShineElementArray(const json_object *new_obj, ShineElement_t **element, uint16 *number)
{
	ShineElement_t *pelement;
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;

	int i;
	int arraylen = json_object_array_length((json_object *)new_obj);

	if (arraylen < 0)
	{
		arraylen = 0;
		dprif("ShineElement array number error set to 0");
	}

	*number = arraylen;
	*element = malloc(sizeof(ShineElement_t)*arraylen);
	memset(*element, 0, sizeof(ShineElement_t)*arraylen);

	pelement = *element;

	for (i = 0; i < arraylen; i++)
	{
		dprif("* ShineElement %d\n", i);
		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "locusTotal"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					pelement[i].LocusTotal = vint;
					dprif("LocusTotal : %d\n", pelement[i].LocusTotal);
				}
				else
				{
					dprif("LocusTotal error,current value %d\n", pelement[i].LocusTotal);
				}
			}
			else if (!strcmp(key, "startX"))
			{
				vint = json_object_get_int(val);
				pelement[i].StartX = vint;
				dprif("StartX : %d\n", pelement[i].StartX);
			}
			else if (!strcmp(key, "startY"))
			{
				vint = json_object_get_int(val);
				pelement[i].StartY = vint;
				dprif("StartY : %d\n", pelement[i].StartY);
			}
			else if (!strcmp(key, "startPictureNumber"))
			{
				vint = json_object_get_int(val);
				pelement[i].StartPictureNumber = vint;
				dprif("StartPictureNumber : %d\n", pelement[i].StartPictureNumber);
			}
			else if (!strcmp(key, "locusArray"))
			{
				JsonParse_ShineLocusArray(val, &pelement[i].LocusPtr, &pelement[i].LocusTotal);
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
int JsonParse_ShineLocusArray(const json_object *new_obj, ShineLocus_t **locus, uint16 *number)
{
	ShineLocus_t *plocus;
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;

	int i;
	int arraylen = json_object_array_length((json_object *)new_obj);

	if (arraylen < 0)
	{
		arraylen = 0;
		dprif("ShineLocus array number error set to 0");
	}

	*number = arraylen;
	*locus = malloc(sizeof(ShineLocus_t)*arraylen);
	memset(*locus, 0, sizeof(ShineLocus_t)*arraylen);

	plocus = *locus;

	for (i = 0; i < arraylen; i++)
	{
		dprif("* ShineLocus %d\n", i);
		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "pictureNumber"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					plocus[i].PictureNumber = vint;
					dprif("PictureNumber : %d\n", plocus[i].PictureNumber);
				}
				else
				{
					dprif("PictureNumber error,current value %d\n", plocus[i].PictureNumber);
				}
			}
			else if (!strcmp(key, "offsetX"))
			{
				vint = json_object_get_int(val);
				plocus[i].OffsetX = vint;
				dprif("OffsetX : %d\n", plocus[i].OffsetX);
			}
			else if (!strcmp(key, "offsetY"))
			{
				vint = json_object_get_int(val);
				plocus[i].OffsetY = vint;
				dprif("OffsetY : %d\n", plocus[i].OffsetY);
			}
			else if (!strcmp(key, "recycleTotal"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					plocus[i].RecycleTotal = vint;
					dprif("RecycleTotal : %d\n", plocus[i].RecycleTotal);
				}
				else
				{
					dprif("RecycleTotal error,current value %d\n", plocus[i].RecycleTotal);
				}
			}
			else if (!strcmp(key, "nextPicture"))
			{
				vbool = json_object_get_boolean(val);
				plocus[i].NextPicture = vbool;
				dprif("NextPicture : %d\n", plocus[i].NextPicture);
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
int Inspect_ProgramDescribe(ProgramDescribe_t *program, Record_t *record)
{
	dprif("* Inspect_ProgramDescribe\n");

	dprif("ProgramErrorno : %d\n", record->ProgramErrorno);

	return record->ProgramErrorno;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_AreaArray(const json_object *new_obj, Area_t **area, uint16 *number, char *id, Record_t *record)
{
	Area_t *parea;
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;
	int i;
	int arraylen = json_object_array_length((json_object *)new_obj);

	if (arraylen < 0)
	{
		arraylen = 0;
		dprif("Area array number error set to 0");
	}

	*number = arraylen;
	*area = malloc(sizeof(Area_t)*arraylen);
	memset(*area, 0, sizeof(Area_t)*arraylen);

	parea = *area;

	for (i = 0; i < arraylen; i++)
	{
		dprif("<<<<<<<<<<<< AreaArray >>>>>>>>>>>>\n");
		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "name"))
			{
				strcpy(vstring, json_object_get_string(val));
				if ((strlen(vstring) < SHORT_NAME_LEN) && (strcmp(vstring, "none") != 0))
				{
					strcpy(parea[i].Name, vstring);
					dprif("Name : %s\n", parea[i].Name);
				}
				else
				{
					sprintf(parea[i].Name, "%s_a%d.acf", id, i);
					dprif("Name : %s (default set)\n", parea[i].Name);
				}
			}
			else if (!strcmp(key, "type"))
			{
				strcpy(vstring, json_object_get_string(val));
				if(!strcmp(vstring, "word"))
					parea[i].Type = AREA_WORD;
				else if(!strcmp(vstring, "time"))
					parea[i].Type = AREA_TIME;
				else if(!strcmp(vstring, "watch"))
					parea[i].Type = AREA_WATCH;
				else if(!strcmp(vstring, "shine"))
					parea[i].Type = AREA_SHINE;
				else if(!strcmp(vstring, "count"))
					parea[i].Type = AREA_COUNT;
				else if(!strcmp(vstring, "sensor"))
					parea[i].Type = AREA_SENSOR;
				else if(!strcmp(vstring, "colorful"))
					parea[i].Type = AREA_COLORFUL;
				else
				{
					dprif("Type key error\n");
				}

				dprif("Type : %d\n", parea[i].Type);
			}
			else if (!strcmp(key, "coordX"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					parea[i].Coord.X = vint;
					dprif("Coord.X : %d\n", parea[i].Coord.X);
				}
				else
				{
					dprif("Coord.X error,current value %d\n", parea[i].Coord.X);
				}
			}
			else if (!strcmp(key, "coordY"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					parea[i].Coord.Y = vint;
					dprif("Coord.Y : %d\n", parea[i].Coord.Y);
				}
				else
				{
					dprif("Coord.Y error,current value %d\n", parea[i].Coord.Y);
				}
			}
			else if (!strcmp(key, "coordWidth"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					parea[i].Coord.Width = vint;
					dprif("Coord.Width : %d\n", parea[i].Coord.Width);
				}
				else
				{
					dprif("Coord.Width error,current value %d\n", parea[i].Coord.Width);
				}
			}
			else if (!strcmp(key, "coordHeight"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					parea[i].Coord.Height = vint;
					dprif("Coord.Height : %d\n", parea[i].Coord.Height);
				}
				else
				{
					dprif("Coord.Height error,current value %d\n", parea[i].Coord.Height);
				}
			}
			else if (!strcmp(key, "waterEdging"))
			{
				dprif("* WaterEdging\n");
				json_object_object_foreach(val, key_1, val_1)
				{
					if (!strcmp(key_1, "width"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							parea[i].WaterEdging.Width = vint;
							dprif("Width : %d\n", parea[i].WaterEdging.Width);
						}
						else
						{
							dprif("Width error,current value %d\n", parea[i].WaterEdging.Width);
						}
					}
					else if (!strcmp(key_1, "height"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							parea[i].WaterEdging.Height = vint;
							dprif("Height : %d\n", parea[i].WaterEdging.Height);
						}
						else
						{
							dprif("Height error,current value %d\n", parea[i].WaterEdging.Height);
						}
					}
					else if (!strcmp(key_1, "speed"))
					{
						vint = json_object_get_int(val_1);
						if (vint >= 0)
						{
							parea[i].WaterEdging.Speed = vint;
							dprif("Speed : %d\n", parea[i].WaterEdging.Speed);
						}
						else
						{
							dprif("Speed error,current value %d\n", parea[i].WaterEdging.Speed);
						}
					}
					else if (!strcmp(key_1, "flicker"))
					{
						vbool = json_object_get_boolean(val_1);
						if (vbool == TRUE)
						{
							parea[i].WaterEdging.Mode |= WATER_EDGING_FLICKER;
						}
						else
						{
							parea[i].WaterEdging.Mode &= ~WATER_EDGING_FLICKER;
						}

						dprif("Mode : %.2X\n", parea[i].WaterEdging.Mode);
					}
					else if (!strcmp(key_1, "mode"))
					{
						strcpy(vstring, json_object_get_string(val_1));

						if(!strcmp(vstring, "forward"))
						{
							parea[i].WaterEdging.Mode |= WATER_EDGING_MODE(WE_FORWARD);
							parea[i].WaterEdging.Mode |= WATER_EDGING_MODE_ENABLE;
						}
						else if(!strcmp(vstring, "reversal"))
						{
							parea[i].WaterEdging.Mode |= WATER_EDGING_MODE(WE_REVERSAL);
							parea[i].WaterEdging.Mode |= WATER_EDGING_MODE_ENABLE;
						}
						else if(!strcmp(vstring, "static"))
						{
							parea[i].WaterEdging.Mode |= WATER_EDGING_MODE(WE_STATIC);
							parea[i].WaterEdging.Mode |= WATER_EDGING_MODE_ENABLE;
						}
						else if(!strcmp(vstring, "none"))
						{
							parea[i].WaterEdging.Mode &= ~WATER_EDGING_MODE_ENABLE;
						}
						else
						{
							dprif("WaterEdging.Mode key error\n");
						}

						dprif("Mode : %.2X\n", parea[i].WaterEdging.Mode);
					}
					else if (!strcmp(key_1, "pictureUpContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(parea[i].WaterEdging.PictureUpContent, vstring);
							dprif("PictureUpContent : %s\n", parea[i].WaterEdging.PictureUpContent);
						}
						else
						{
							dprif("PictureUpContent critical error\n");
						}
					}
					else if (!strcmp(key_1, "pictureDownContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(parea[i].WaterEdging.PictureDownContent, vstring);
							dprif("PictureDownContent : %s\n", parea[i].WaterEdging.PictureDownContent);
						}
						else
						{
							dprif("PictureDownContent critical error\n");
						}
					}
					else if (!strcmp(key_1, "pictureLeftContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(parea[i].WaterEdging.PictureLeftContent, vstring);
							dprif("PictureLeftContent : %s\n", parea[i].WaterEdging.PictureLeftContent);
						}
						else
						{
							dprif("PictureLeftContent critical error\n");
						}
					}
					else if (!strcmp(key_1, "pictureRightContent"))
					{
						strcpy(vstring, json_object_get_string(val_1));
						if (strlen(vstring) < PATH_LEN)
						{
							strcpy(parea[i].WaterEdging.PictureRightContent, vstring);
							dprif("PictureRightContent : %s\n", parea[i].WaterEdging.PictureRightContent);
						}
						else
						{
							dprif("PictureRightContent critical error\n");
						}
					}
				}
			}
		}
	}

	for (i = 0; i < arraylen; i++)
	{
		if (parea[i].Type == 0)
		{
			dprif("parea[%d].Type error\n", i);
			record->ProgramErrorno = RECORD_ERRORNO_KEYLACK;
			return -1;
		}

		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "areaDescribe"))
			{
				if (parea[i].Type == AREA_WORD)
					JsonParse_ACFWordConfig(val, (ACFWord_t **)&parea[i].Ptr, record);
				else if (parea[i].Type == AREA_TIME)
#if defined TIME_JSON_MODE_1
					JsonParse_ACFTimeConfig(val, (ACFTime_t **)&parea[i].Ptr, record);
#elif defined TIME_JSON_MODE_2
					JsonParse_ACFTimeConfig2(val, (Coord_t *)&parea[i].Coord, (WaterEdging_t*)&parea[i].WaterEdging, (ACFTime_t **)&parea[i].Ptr, record);
#endif
				else if (parea[i].Type == AREA_SHINE)
					JsonParse_ACFShineConfig(val, (ACFShine_t **)&parea[i].Ptr, record);
				else if (parea[i].Type == AREA_WATCH)
					JsonParse_ACFWatchConfig(val, (ACFWatch_t **)&parea[i].Ptr, record);
				else if (parea[i].Type == AREA_COUNT)
					JsonParse_ACFCountConfig(val, (ACFCount_t **)&parea[i].Ptr, record);
				else if (parea[i].Type == AREA_SENSOR)
					JsonParse_ACFTemperatureConfig(val, (ACFTemperature_t **)&parea[i].Ptr, record);
				else if (parea[i].Type == AREA_COLORFUL)
					JsonParse_ACFColorfulConfig(val, (ACFColorful_t **)&parea[i].Ptr, record);
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
int FwriteWaterEdging(FILE *fp, uint32 offset, const WaterEdging_t *edging, const PictureKarma_t *karma, uint32 *wsize)
{
	int ret;
	uint32 cnt;
	Coord_t coord;
	uint32 size;
	uint16 *dotbuf = NULL;

	if ((fp == NULL) || (edging == NULL) || (wsize == NULL))
	{
		*wsize = 0;
		dprif("FwriteWaterEdging parameter error\n");
		return -1;
	}

	*wsize = 0;

	fseek(fp, offset, SEEK_SET);
	ret = PictureToMallocDotArray(edging->PictureUpContent, &dotbuf, &size, &coord, karma);
	if (ret != 0)
		return -1;
	else
		PrintDotArray(dotbuf, &coord, karma);
	cnt = fwrite(dotbuf, 1, size, fp);
	if (dotbuf != NULL)
	{
		free(dotbuf);
		dotbuf = NULL;
	}
	*wsize += size;

	ret = PictureToMallocDotArray(edging->PictureDownContent, &dotbuf, &size, &coord, karma);
	if (ret != 0)
		return -1;
	else
		PrintDotArray(dotbuf, &coord, karma);
	cnt = fwrite(dotbuf, 1, size, fp);
	if (dotbuf != NULL)
	{
		free(dotbuf);
		dotbuf = NULL;
	}
	*wsize += size;

	ret = PictureToMallocDotArray(edging->PictureLeftContent, &dotbuf, &size, &coord, karma);
	if (ret != 0)
		return -1;
	else
		PrintDotArray(dotbuf, &coord, karma);
	cnt = fwrite(dotbuf, 1, size, fp);
	if (dotbuf != NULL)
	{
		free(dotbuf);
		dotbuf = NULL;
	}
	*wsize += size;

	ret = PictureToMallocDotArray(edging->PictureRightContent, &dotbuf, &size, &coord, karma);
	if (ret != 0)
		return -1;
	else
		PrintDotArray(dotbuf, &coord, karma);
	cnt = fwrite(dotbuf, 1, size, fp);
	if (dotbuf != NULL)
	{
		free(dotbuf);
		dotbuf = NULL;
	}
	*wsize += size;

	return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int FwritePicture(FILE *fp, uint32 offset, const char *content, const PictureKarma_t *karma, uint32 *wsize)
{
	Coord_t coord;
	uint32 cnt;
	uint32 size;
	uint16 *dotbuf = NULL;

	if ((fp == NULL) || (content == NULL) || (wsize == NULL))
	{
		*wsize = 0;
		dprif("FwriteWaterEdging parameter error\n");
		return -1;
	}

	fseek(fp, offset, SEEK_SET);
	if (PictureToMallocDotArray(content, &dotbuf, &size, &coord, karma) == 0)
		PrintDotArray(dotbuf, &coord, karma);
	cnt = fwrite(dotbuf, 1, size, fp);
	if (dotbuf != NULL)
		free(dotbuf);

	*wsize = size;

	return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int FwriteFile(FILE *fp, uint32 offset, const char *file, uint32 *wsize)
{
	Coord_t coord;
	uint32 i;
	uint32 size;
	FILE *filefp;
	uint32 filesize;
	char buf[512];

	if ((fp == NULL) || (file == NULL) || (wsize == NULL))
	{
		*wsize = 0;
		dprif("FwriteFile parameter error\n");
		return -1;
	}

	if ((filefp = fopen(file, "rb")) == NULL)
	{
		*wsize = 0;
		printf("FwriteFile fopen %s fail\n", file);
		return -1;
	}

	filesize = GetFileSizeK(filefp);

	fseek(fp, offset, SEEK_SET);
	fseek(filefp, 0, SEEK_SET);

	for (i = 0; i < filesize/512; i++)
	{
		fread(buf, 1, 512, filefp);
		fwrite(buf, 1, 512, fp);
	}

	fread(buf, 1, filesize%512, filefp);
	fwrite(buf, 1, filesize%512, fp);

	*wsize = filesize;

	fclose(filefp);

	return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CreatFile_ProgramCCF(const char *path, CCF_t *ccf, uint8 endian, Record_t *record)
{
	uint32 cnt;
	FILE *fp = NULL;
	FileHeader_t header;

	dprif("CreatFile_ProgramCCF\n");

	if ((path == NULL) || (ccf == NULL))
	{
		dprif("CreatFile_ProgramCCF parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ProgramCCF path error\n");
		goto out;
	}

	if (CreateDirectoryK(path) < 0)
		goto out;

	char file[PATH_LEN];

	//sprintf(file, "%s%s", path, "program.ccf");
	//strcpy(ccf->Name, "program.ccf");
	sprintf(file, "%s%s", path, "ALL.ccf");
	strcpy(ccf->Name, "ALL.ccf");
	memcpy(header.Sign, "CCF", 3);

	if ((fp = fopen(file, "wb+")) == NULL)
	{
		dprif("fopen %s failed\n", file);
		goto out;
	}

	//-----------------------------------------------------------------------//
	fseek(fp, sizeof(FileHeader_t), SEEK_SET);
	cnt = fwrite(&ccf->ProgramTotal, sizeof(uint16), 1, fp);
	fseek(fp, sizeof(FileHeader_t) + sizeof(uint16), SEEK_SET);
	cnt = fwrite(ccf->ProgramPtr, sizeof(CCFProgram_t), ccf->ProgramTotal, fp);

	header.Version = FILE_VERSION;
	header.Length = sizeof(FileHeader_t) + sizeof(uint16) + ccf->ProgramTotal*sizeof(CCFProgram_t);
	header.Reserved = 0;
	fseek(fp, 0, SEEK_SET);
	cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

	fclose(fp);

	dprif("CreatFile_ProgramCCF OK!\n");

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
int CreatFile_ProgramBCF(const char *path, BCF_t *bcf, const PictureKarma_t *karma, Record_t *record)
{
	int i;
	uint32 cnt;
	FILE *fp = NULL;
	FileHeader_t header;
	uint32 wcnt = 0;

	dprif("CreatFile_ProgramBCF\n");

	if ((path == NULL) || (bcf == NULL))
	{
		dprif("CreatFile_ProgramBCF parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ProgramBCF path error\n");
		goto out;
	}

	if (CreateDirectoryK(path) < 0)
		goto out;

	char file[PATH_LEN];

	sprintf(file, "%s%s", path, bcf->Name);
	memcpy(header.Sign, "BCF", 3);

	if ((fp = fopen(file, "wb+")) == NULL)
	{
		dprif("fopen %s failed\n", file);
		goto out;
	}

	//-----------------------------------------------------------------------//
	wcnt += sizeof(FileHeader_t) + sizeof(BCF_t) - BCF_ADDED_LENGTH;

	bcf->ElementAddr = wcnt;
	wcnt += bcf->ElementTotal * (sizeof(ShineElement_t) - SHINE_ELEMENT_ADDED_LENGTH);

	bcf->PictureAddr = wcnt;
	wcnt += bcf->PictureTotal * (sizeof(ShinePicture_t) - SHINE_PICTURE_ADDED_LENGTH);

	dprif("Addr %d %d\n", bcf->ElementAddr, bcf->PictureAddr);

	for (i = 0; i < bcf->ElementTotal; i++)
	{
		bcf->ElementPtr[i].LocusOffset = wcnt;
		dprif("LocusOffset %d %d %d\n", i, bcf->ElementPtr[i].LocusOffset, bcf->ElementPtr[i].LocusOffset%4);
		wcnt += bcf->ElementPtr[i].LocusTotal * sizeof(ShineLocus_t);
	}

	for (i = 0; i < bcf->PictureTotal; i++)
	{
		bcf->PicturePtr[i].PictureOffset = wcnt;
		dprif("PictureOffset %d %d %d\n", i, bcf->PicturePtr[i].PictureOffset, bcf->PicturePtr[i].PictureOffset%4);
		FwritePicture(fp, bcf->PicturePtr[i].PictureOffset, bcf->PicturePtr[i].PictureContent, karma, &cnt);
		wcnt += cnt;
	}

	fseek(fp, sizeof(FileHeader_t), SEEK_SET);
	cnt = fwrite(bcf, sizeof(BCF_t) - BCF_ADDED_LENGTH, 1, fp);

	fseek(fp, bcf->ElementAddr, SEEK_SET);
	for (i = 0; i < bcf->ElementTotal; i++)
	{
		cnt = fwrite(&bcf->ElementPtr[i], sizeof(ShineElement_t) - SHINE_ELEMENT_ADDED_LENGTH, 1, fp);
	}

	fseek(fp, bcf->PictureAddr, SEEK_SET);
	for (i = 0; i < bcf->PictureTotal; i++)
	{
		cnt = fwrite(&bcf->PicturePtr[i], sizeof(ShinePicture_t) - SHINE_PICTURE_ADDED_LENGTH, 1, fp);
	}

	for (i = 0; i < bcf->ElementTotal; i++)
	{
		fseek(fp, bcf->ElementPtr[i].LocusOffset, SEEK_SET);
		cnt = fwrite(bcf->ElementPtr[i].LocusPtr, sizeof(ShineLocus_t), bcf->ElementPtr[i].LocusTotal, fp);
	}

	header.Version = FILE_VERSION;
	header.Length = wcnt;
	header.Reserved = 0;
	fseek(fp, 0, SEEK_SET);
	cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

	fclose(fp);

	dprif("CreatFile_ProgramBCF OK!\n");

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
int CreatFile_ProgramPCF(const char *path, PCF_t *pcf, const PictureKarma_t *karma, Record_t *record)
{
	int i;
	uint32 cnt;
	FILE *fp = NULL;
	FileHeader_t header;
	uint32 wcnt = 0;

	dprif("CreatFile_ProgramPCF\n");

	if ((path == NULL) || (pcf == NULL))
	{
		dprif("CreatFile_ProgramPCF parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ProgramPCF path error\n");
		goto out;
	}

	if (CreateDirectoryK(path) < 0)
		goto out;

	char file[PATH_LEN];

	sprintf(file, "%s%s", path, pcf->Name);
	memcpy(header.Sign, "PCF", 3);

	if ((fp = fopen(file, "wb+")) == NULL)
	{
		dprif("fopen %s failed\n", file);
		goto out;
	}

	//-----------------------------------------------------------------------//
	uint32 area_name_addr;

	wcnt += sizeof(FileHeader_t) + sizeof(PCF_t) - (PCF_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
	area_name_addr = wcnt;

	wcnt += pcf->AreaTotal * SHORT_NAME_LEN;
	pcf->WaterEdging.PictureOffset = wcnt;
	if (pcf->WaterEdging.Mode & WATER_EDGING_MODE_ENABLE)
	{
		FwriteWaterEdging(fp, pcf->WaterEdging.PictureOffset, &pcf->WaterEdging, karma, &cnt);
		wcnt += cnt;
	}

	if ((pcf->WaterEdging.Mode & WATER_EDGING_MODE_ENABLE) == 0)
	{
		memset(&pcf->WaterEdging, 0, sizeof(WaterEdging_t)- WATER_EDGING_ADDED_LENGTH);
	}
	fseek(fp, sizeof(FileHeader_t), SEEK_SET);
	cnt = fwrite(pcf, sizeof(PCF_t) - (PCF_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH), 1, fp);

	fseek(fp, area_name_addr, SEEK_SET);
	for (i = 0; i < pcf->AreaTotal; i++)
	{
		cnt = fwrite(pcf->Area[i].Name, SHORT_NAME_LEN, 1, fp);
	}

	header.Version = FILE_VERSION;
	header.Length = wcnt;
	header.Reserved = 0;
	fseek(fp, 0, SEEK_SET);
	cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

	fclose(fp);

	dprif("CreatFile_ProgramPCF OK!\n");

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
#define FILE_DESCRIBE_ADDED_LENGTH              (sizeof(uint32))
#pragma pack(push, 1)
typedef struct _FileDescribe_
{
	uint32 Offset;
	char Name[SHORT_NAME_LEN];
	// -4
	uint32 Size;
} FileDescribe_t;
#pragma pack(pop)

int CreatFile_ProgramConcord(const char *path, const ProgramDescribe_t *program, uint8 endian, Record_t *record)
{
	int i, j;
	int filecnt = 0;
	uint32 filetotal = 0;
	uint32 cnt;
	FILE *fp = NULL;
	FileHeader_t header;
	uint32 wcnt = 0;

	dprif("CreatFile_ProgramConcord\n");

	if ((path == NULL) || (program == NULL))
	{
		dprif("CreatFile_ProgramConcord parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ProgramConcord path error\n");
		goto out;
	}

	if (CreateDirectoryK(path) < 0)
		goto out;

	char file[PATH_LEN];

	if (endian == FILE_LITTLE_ENDIAN)
	{
		sprintf(file, "%s%s", path, "ALL.rcf");
		memcpy(header.Sign, "RCF", 3);
	}
	else
	{
		sprintf(file, "%s%s", path, "ALL.rcx");
		memcpy(header.Sign, "RCX", 3);
	}

	if ((fp = fopen(file, "wb+")) == NULL)
	{
		dprif("fopen %s failed\n", file);
		goto out;
	}

	//-----------------------------------------------------------------------//
	FileDescribe_t *filetable;
	uint32 file_describe_addr = 0;
	uint32 size;

	filetotal += 1; //CCF
	for (i = 0; i < program->CCF.ProgramTotal; i++)
	{
		filetotal += 2; //PCF,BCF
		filetotal += program->PCF[i].AreaTotal; //ACF
	}

	filetable = malloc(filetotal * sizeof(FileDescribe_t));

	fseek(fp, sizeof(FileHeader_t), SEEK_SET);
	cnt = fwrite(&filetotal, sizeof(uint16), 1, fp);

	wcnt += sizeof(FileHeader_t) + sizeof(uint16);
	file_describe_addr = wcnt;

	wcnt += filetotal * (sizeof(FileDescribe_t) - FILE_DESCRIBE_ADDED_LENGTH);

	sprintf(file, "%s%s", path, program->CCF.Name);
	strcpy(filetable[filecnt].Name, program->CCF.Name);
	filetable[filecnt].Offset = wcnt;
	FwriteFile(fp, wcnt, file, &size);
	//remove(file);
	wcnt += size;
	filecnt++;

	for (i = 0; i < program->CCF.ProgramTotal; i++)
	{
		sprintf(file, "%s%s", path, program->PCF[i].Name);
		strcpy(filetable[filecnt].Name, program->PCF[i].Name);
		filetable[filecnt].Offset = wcnt;
		FwriteFile(fp, wcnt, file, &size);
		//remove(file);
		wcnt += size;
		filecnt++;

		sprintf(file, "%s%s", path, program->BCF[i].Name);
		strcpy(filetable[filecnt].Name, program->BCF[i].Name);
		filetable[filecnt].Offset = wcnt;
		FwriteFile(fp, wcnt, file, &size);
		//remove(file);
		wcnt += size;
		filecnt++;

		for (j = 0; j < program->PCF[i].AreaTotal; j++)
		{
			sprintf(file, "%s%s", path, program->PCF[i].Area[j].Name);
			strcpy(filetable[filecnt].Name, program->PCF[i].Area[j].Name);
			filetable[filecnt].Offset = wcnt;
			FwriteFile(fp, wcnt, file, &size);
			//remove(file);
			wcnt += size;
			filecnt++;
		}
	}

	fseek(fp, file_describe_addr, SEEK_SET);
	for (i = 0; i < filetotal; i++)
	{
		cnt = fwrite(&filetable[i], sizeof(FileDescribe_t) - FILE_DESCRIBE_ADDED_LENGTH, 1, fp);
	}

	header.Version = FILE_VERSION;
	header.Length = wcnt;
	header.Reserved = 0;
	fseek(fp, 0, SEEK_SET);
	cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

	free(filetable);

	fclose(fp);

	dprif("CreatFile_ProgramConcord OK!\n");

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
int FreeMemory_ProgramCCF(ProgramDescribe_t *program, Record_t *record)
{
	if (program == NULL)
	{
		dprif("FreeMemory_ProgramCCF parameter is NULL\n");
		return -1;
	}

	if (program->CCF.ProgramPtr != NULL)
	{
		free(program->CCF.ProgramPtr);
		program->CCF.ProgramPtr = NULL;
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
int FreeMemory_ProgramBCF(ProgramDescribe_t *program, Record_t *record)
{
	int i;

	if (program == NULL)
	{
		dprif("FreeMemory_ProgramBCF parameter is NULL\n");
		return -1;
	}

	if (program->BCF != NULL)
	{
		for (i = 0; i < program->CCF.ProgramTotal; i++)
		{
			if (program->BCF[i].PicturePtr != NULL)
			{
				free(program->BCF[i].PicturePtr);
				program->BCF[i].PicturePtr = NULL;
			}

			if (program->BCF[i].ElementPtr != NULL)
			{
				int j;
				for (j = 0; j < program->BCF[i].ElementTotal; j++)
				{
					if (program->BCF[i].ElementPtr[j].LocusPtr != 0)
					{
						free(program->BCF[i].ElementPtr[j].LocusPtr);
						program->BCF[i].ElementPtr[j].LocusPtr = NULL;
					}
				}

				free(program->BCF[i].ElementPtr);
				program->BCF[i].ElementPtr = NULL;
			}
		}

		free(program->BCF);
		program->BCF = NULL;
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
int FreeMemory_ProgramPCF(ProgramDescribe_t *program, Record_t *record)
{
	int i;

	if (program == NULL)
	{
		dprif("FreeMemory_ProgramBCF parameter is NULL\n");
		return -1;
	}

	if (program->PCF != NULL)
	{
		for (i = 0; i < program->CCF.ProgramTotal; i++)
		{
			if (program->PCF[i].Area != NULL)
			{
				free(program->PCF[i].Area);
				program->PCF[i].Area = NULL;
			}
		}

		free(program->PCF);
		program->PCF = NULL;
	}

	return 0;
}
