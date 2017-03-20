#ifndef __KALER_CFG_H__
#define __KALER_CFG_H__

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "kaler_macro.h"

/**
 *******************************************************************************
 * typedef type
 *******************************************************************************
 */
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

#define YES                 (1)
#define NO                  (0)

/**
 *******************************************************************************
 * variable parameter define
 *******************************************************************************
 */
#ifndef va_start
typedef void * va_list;

/* _INTSIZEOF sizeof type */
# define _INTSIZEOF(n)		((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
/* VA_START set start parameter */
# define va_start(ap, v)	(ap = (va_list)&v + _INTSIZEOF(v))
/* VA_ARG read parameter */
# define va_arg(ap, t)		(*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
/* VA_END end */
# define va_end(ap)			(ap = (va_list)0)
#endif /* va_start */

/**
 *******************************************************************************
 * little/big endian swap
 *******************************************************************************
 */
#define swap16by8(val) \
	((uint16_t)((((uint16_t)(val) & (uint16_t)0x00ffU) << 8) | \
	(((uint16_t)(val) & (uint16_t)0xff00U) >> 8)))

#define swap32by8(val) \
	((uint32_t)((((uint32_t)(val) & (uint32_t)0x000000ffU) << 24) | \
	(((uint32_t)(val) & (uint32_t)0x0000ff00U) <<  8) | \
	(((uint32_t)(val) & (uint32_t)0x00ff0000U) >>  8) | \
	(((uint32_t)(val) & (uint32_t)0xff000000U) >> 24)))

#define swap32by16(val) \
	((uint32_t)((((uint32_t)(val) & (uint32_t)0x0000ffffU) << 16) | \
	(((uint32_t)(val) & (uint32_t)0xffff0000U) >> 16)))

#endif // __KALER_CFG_H__
