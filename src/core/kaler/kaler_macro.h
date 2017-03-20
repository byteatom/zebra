#ifndef __KALER_MACRO_H__
#define __KALER_MACRO_H__

#include <stdio.h>

/**
 *******************************************************************************
 * endian
 *******************************************************************************
 */
#define FILE_ALL_ENDIAN      (0)
#define FILE_BIG_ENDIAN      (1)
#define FILE_LITTLE_ENDIAN   (2)

/**
 *******************************************************************************
 * length
 *******************************************************************************
 */
#define PATH_LEN            (512)
#define NAME_LEN            (64)
#define JSON_STRING_LEN     (512)
#define SHORT_NAME_LEN      (16)

/**
 *******************************************************************************
 * length
 *******************************************************************************
 */
#define SCREEN_FILE_MAX                 (100)
#define SCREEN_CONFIG_FILE_LENGTH       (78)
#define SCANDOT_CONFIG_FILE_LENGTH      (548)

/**
 *******************************************************************************
 * debug printf
 *******************************************************************************
 */
#define PRINT_DEBUG
#define undprif

#ifdef PRINT_DEBUG
#define dprif(fmt, ...) do {printf("%s %d\t- ",__FILE__, __LINE__); printf(fmt, __VA_ARGS__);fflush(stdout);} while (0)
//#define dprif(fmt, args...) do {printf(fmt, ##args);fflush(stdout);} while (0)
#else
#define dprif(fmt, args...)
#endif

#endif // __KALER_MACRO_H__
