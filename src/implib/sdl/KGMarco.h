#ifndef KGMARCO_H
#define KGMARCO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;
typedef signed char 		int8;
typedef signed short		int16;
typedef signed int   		int32;
typedef signed long long	int64;
typedef float				fp32;
typedef double				fp64;

#define PATH_LEN	(512)

typedef struct _KGSize_ {
	int width;
	int height;
} KGSize;

typedef struct _KGPoint_ {
	int x;
	int y;
} KGPoint;

typedef struct _KGCoord_ {
	int x;
	int y;
	int width;
	int height;
} KGCoord;

typedef enum _KGColor_ {
	KG_COLOR_RED = 0,
	KG_COLOR_GREEN,
        KG_COLOR_YELLOW,
	KG_COLOR_BLUE,
	KG_COLOR_CYAN,
	KG_COLOR_MAGENTA,
	KG_COLOR_WHITE,
	KG_COLOR_BLACK,

	KG_COLOR_PURPLE,
	KG_COLOR_PINK,
	KG_COLOR_PANSY,
	KG_COLOR_ORANGE,
        KG_COLOR_RANDOM,
} KGColor;

typedef enum _KGPicType_ {
	KG_BMP_1BIT = 0,
	KG_BMP_32BIT,
	KG_BMP_24BIT,
	KG_PNG_24BIT,
} KGPicType;

typedef enum _KGCreateMode_ {
	KG_CREATE_FILE = 0,
	KG_CREATE_MEMORY,
} KGCreateMode;

typedef enum _KGFormat_ {
	Format_Invalid,
	Format_Mono,
	Format_MonoLSB,
	Format_Indexed8,
	Format_RGB32,
	Format_ARGB32, //use it
	Format_ARGB32_Premultiplied,
	Format_RGB16,
	Format_ARGB8565_Premultiplied,
	Format_RGB666,
	Format_ARGB6666_Premultiplied,
	Format_RGB555,
	Format_ARGB8555_Premultiplied,
	Format_RGB888,
	Format_RGB444,
	Format_ARGB4444_Premultiplied,
	Format_RGBX8888,
	Format_RGBA8888,
	Format_RGBA8888_Premultiplied,
	Format_BGR30,
	Format_A2BGR30_Premultiplied,
	Format_RGB30,
	Format_A2RGB30_Premultiplied,
} KGFormat;

typedef struct _KGBitmap_ {
	unsigned char *data;
	KGFormat format;
	int size;
	int width;
	int height;
} KGBitmap;

#ifdef __cplusplus
}
#endif

#endif //KGMARCO_H
