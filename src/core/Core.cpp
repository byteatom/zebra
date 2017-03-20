#include "Core.h"

#include <cstring>

extern "C" {
#include "kaler/parse_top.h"
#include "kaler/file_ops.h"
}

DEF_PLUGIN_CREATOR(Core)

bool Core::build(const char* json)
{
	AllConfig_t AllConfig;
	Record_t Record;

	memset(&Record, 0, sizeof(Record_t));

	if (JsonParseAll(json, &AllConfig, &Record) != 0)
	{
		return false;
	}

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

	return true;
}
