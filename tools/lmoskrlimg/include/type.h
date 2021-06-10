/**********************************************************
		数据类型文件type.h
***********************************************************
				彭东 ＠ 2010.10.31.09.32
**********************************************************/
#ifndef _TYPE_H
#define _TYPE_H
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;
typedef char s8_t;
typedef short s16_t;
typedef int s32_t;
typedef long s64_t;
typedef long sint_t;
typedef unsigned long uint_t;
typedef unsigned long adr_t;
typedef unsigned long size_t;
typedef u64_t bool_t;
typedef u64_t dev_t;
typedef const char *str_t;
typedef char char_t;
#define KLINE static inline
#define ALIGN(x, a) (((x) + (a)-1) & ~((a)-1))
#define BLK_ALIGN(x) ALIGN(x, 0x1000)
#define TRUE 1
#define FALSE 0

#endif
