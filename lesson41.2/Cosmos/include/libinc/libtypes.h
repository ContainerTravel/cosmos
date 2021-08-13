#ifndef _LIBTYPES_H
#define _LIBTYPES_H
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef char s8_t;
typedef short s16_t;
typedef int s32_t;
typedef long s64_t;
typedef long sint_t;
typedef char char_t;
typedef unsigned long uint_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;
typedef sint_t sysstus_t;
typedef sint_t hand_t;
typedef void* buf_t;
typedef unsigned long size_t;

typedef struct s_TIME
{
    
    uint_t      year;
    uint_t      mon;
    uint_t      day;
    uint_t      date;
    uint_t      hour;
    uint_t      min;
    uint_t      sec;
    
}times_t;


#define SYSSTUSERR (-1)
#define SYSSTUSOK (0)
#define NULL 0
#define FILE_TY_MASK 0xf
#define FILE_TY_FILE 0
#define FILE_TY_DEV 1
#define RWO_FLG_MASK 0xff
#define RWO_FLG_BITS 0x4

#define RW_FLG (0x3<<RWO_FLG_BITS)
#define RO_FLG (0x1<<RWO_FLG_BITS)
#define WO_FLG (0x2<<RWO_FLG_BITS)
#define NF_FLG (0x4<<RWO_FLG_BITS)

#define INR_MM_ALLOC 0x1UL
#define INR_MM_FREE 0x2UL
#define INR_TD_EXEL 0x3UL
#define INR_TD_EXIT 0x4UL
#define INR_TD_HAND 0x5UL
#define INR_TD_RSTATS 0x6UL
#define INR_TD_SSTATS 0x7UL
#define INR_FS_OPEN 0x8UL
#define INR_FS_CLOSE 0x9UL
#define INR_FS_READ 0xaUL
#define INR_FS_WRITE 0xbUL
#define INR_FS_IOCTRL 0xcUL
#define INR_FS_LSEEK 0xdUL
#define INR_TIME 0xeUL
#define INR_TD_TICK 0xfUL


#endif // LIBTYPES_H
