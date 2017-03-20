#include "kaler_config.h"
#include "../json/json.h"
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
int JsonParse_ACFColorfulPictureFuseArray(const json_object *new_obj, PictureFuse2_t **picfuse, uint16 *number);
int JsonParse_ACFColorfulConfig(const json_object *new_obj, ACFColorful_t **acf, Record_t *record)
{
	char vstring[JSON_STRING_LEN];
	jboolean vbool;
	int vint;
	ACFColorful_t *pacf;

	*acf = malloc(sizeof(ACFColorful_t));
	memset(*acf, 0, sizeof(ACFColorful_t));

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
		if (!strcmp(key, "speed"))
		{
			vint = json_object_get_int(val);
			if (vint >= 0)
			{
				pacf->Speed = vint;
				dprif("Speed : %d\n", pacf->Speed);
			}
			else
			{
				dprif("Speed error,current value %d\n", pacf->Speed);
			}
		}
		else if (!strcmp(key, "display"))
		{
			strcpy(vstring, json_object_get_string(val));
			if (strlen(vstring) < PATH_LEN)
			{
				if (!strcmp(vstring, "background"))
					pacf->Display = 0;
				else
					pacf->Display = 1;
				dprif("Display : %d\n", pacf->Display);
			}
			else
			{
				dprif("Display error,current value %d\n", pacf->display);
			}
		}
		else if (!strcmp(key, "pictureFuseArray"))
		{
			JsonParse_ACFColorfulPictureFuseArray(val,
												  &pacf->PictureFusePtr,
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
int JsonParse_ACFColorfulPictureFuseArray(const json_object *new_obj, PictureFuse2_t **picfuse, uint16 *number)
{
	PictureFuse2_t *pfuse;
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

	*picfuse = malloc(sizeof(PictureFuse2_t)*arraylen);
	memset(*picfuse, 0, sizeof(PictureFuse2_t)*arraylen);

	pfuse = *picfuse;

	for (i = 0; i < arraylen; i++)
	{
		dprif("----> ACFPicture : %d\n", i);

		json_object_object_foreach(json_object_array_get_idx((json_object *)new_obj, i), key, val)
		{
			if (!strcmp(key, "startX"))
			{
				vint = json_object_get_int(val);
				pfuse[i].StartX = vint;
				dprif("startX : %d\n", pfuse[i].StartX);
			}
			else if (!strcmp(key, "startY"))
			{
				vint = json_object_get_int(val);
				pfuse[i].StartY = vint;
				dprif("startY : %d\n", pfuse[i].StartY);
			}
			else if (!strcmp(key, "width"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					pfuse[i].Width = vint;
					dprif("Width : %d\n", pfuse[i].Width);
				}
				else
				{
					dprif("Width error,current value %d\n", pfuse[i].Width);
				}
			}
			else if (!strcmp(key, "height"))
			{
				vint = json_object_get_int(val);
				if (vint >= 0)
				{
					pfuse[i].Height = vint;
					dprif("Height : %d\n", pfuse[i].Height);
				}
				else
				{
					dprif("Height error,current value %d\n", pfuse[i].Height);
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
				dprif("ACFColorfulConfig key \"%s\" can not recognized\n", key);
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
int CreatFile_ACFColorful(const char *path, Area_t *area, const PictureKarma_t *karma, Record_t *record)
{
	int i;
	uint32 cnt;
	FILE *fp = NULL;
	FileHeader_t header;
	uint32 wcnt = 0;

	dprif("CreatFile_ACFColorful\n");

	if ((path == NULL) || (area == NULL))
	{
		dprif("CreatFile_ACFColorful parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_ACFColorful path error\n");
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
	ACFColorful_t *acf = area->Ptr;
	uint32 specific_para_addr;
	uint32 picture_para_addr;

	wcnt += sizeof(FileHeader_t) + sizeof(Area_t) - (AREA_ADDED_LENGTH + WATER_EDGING_ADDED_LENGTH);
	specific_para_addr = wcnt;

	wcnt += sizeof(ACFColorful_t) - ACF_COLORFUL_ADDED_LENGTH;
	picture_para_addr = wcnt;
	area->PictureAddr = wcnt;

	wcnt += acf->PictureTotal * (sizeof(PictureFuse2_t) - PICTURE_FUSE2_ADDED_LENGTH);
	for (i = 0; i < acf->PictureTotal; i++)
	{
		acf->PictureFusePtr[i].PictureOffset = wcnt;
		FwritePicture(fp, acf->PictureFusePtr[i].PictureOffset, acf->PictureFusePtr[i].PictureContent, karma, &cnt);
		wcnt += cnt;
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
	cnt = fwrite(acf, sizeof(ACFColorful_t) - ACF_COLORFUL_ADDED_LENGTH, 1, fp);

	fseek(fp, picture_para_addr, SEEK_SET);
	for (i = 0; i < acf->PictureTotal; i++)
	{
		cnt = fwrite(&acf->PictureFusePtr[i], sizeof(PictureFuse2_t) - PICTURE_FUSE2_ADDED_LENGTH, 1, fp);
	}

	header.Version = FILE_VERSION;
	header.Length = wcnt;
	header.Reserved = 0;
	fseek(fp, 0, SEEK_SET);
	cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

	fclose(fp);

	dprif("CreatFile_ACFColorful OK!\n");

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
int FreeMemory_ACFColorful(Area_t *area, Record_t *record)
{
	ACFColorful_t *acf;

	if (area == NULL)
	{
		dprif("FreeMemory_ACFColorful parameter is NULL\n");
		return -1;
	}

	if (area->Type != AREA_COLORFUL)
	{
		dprif("FreeMemory_ACFColorful type is error\n");
		return -2;
	}

	if (area->Ptr != NULL)
	{
		acf = area->Ptr;

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
