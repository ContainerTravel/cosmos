/**********************************************************
        基本数据类型文件bastype_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef _BASTYPE_T_H
#define _BASTYPE_T_H
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
typedef u64_t cpuflg_t;
typedef unsigned long adr_t;

typedef sint_t bool_t;
typedef u32_t dev_t;
typedef const char* str_t;
typedef char char_t;
typedef unsigned long drv_t;
typedef unsigned long mrv_t;
typedef sint_t drvstus_t;
typedef sint_t sysstus_t;
typedef sint_t hand_t;
typedef void* buf_t;
typedef unsigned long size_t;
typedef u64_t reg_t;


typedef void (*inthandler_t)();
typedef drv_t (*i_handle_t)(uint_t int_nr);
typedef drv_t (*f_handle_t)(uint_t int_nr,void* sframe);
typedef drvstus_t (*intflthandle_t)(uint_t ift_nr,void* device,void* sframe);
typedef u64_t mmstus_t;

#define KLINE static inline
#define PUBLIC
#define private	static
#define EXTERN extern
#define KEXTERN extern
#define NULL	0
#define TRUE    1
#define	FALSE	0
#define DFCERRSTUS (-1)
#define DFCOKSTUS (0)
#define NO_HAND (-1)
#define ALIGN(x, a)     (((x) + (a) - 1) & ~((a) - 1))

#define LKHEAD_T __attribute__((section(".head.text")))
#define LKHEAD_D __attribute__((section(".head.data")))
#define LKINIT 

#define EOK 		0
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */
#define EALLOC		35 
#define ENOOBJ 		36
#define EGOON		37 /* go on*/
#define ECPLT		38 /*Complete*/
#define EPARAM		39 //参数错误

#endif
