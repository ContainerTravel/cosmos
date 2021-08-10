/**********************************************************
        内核服务头文件krlservice_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef KRLSERVICE_T_H
#define KRLSERVICE_T_H

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
#define INR_MAX 0x10UL

#define SYSSTUSERR (-1)
#define SYSSTUSOK (0)
#define FILE_TY_MASK 0xf
#define FILE_TY_FILE 0
#define FILE_TY_DEV 1
#define RWO_FLG_MASK 0xff
#define RWO_FLG_BITS 0x4
#define RW_FLG 0x3
#define RO_FLG 0x1
#define WO_FLG 0x2
#define NF_FLG 0x4

typedef struct s_STKPARAME
{
    u64_t gs;
	u64_t fs;
	u64_t es;
	u64_t ds;
	u64_t r15;
	u64_t r14;
	u64_t r13;
	u64_t r12;
	u64_t r11;
	u64_t r10;
	u64_t r9;
	u64_t r8;
	
	u64_t parmv5;//rdi;
	u64_t parmv4;//rsi;
	u64_t rbp;
	u64_t parmv3;//rdx;
	u64_t parmv2;//rcx;
	u64_t parmv1;//rbx;

    u64_t rvsrip;    
    u64_t rvscs;
    u64_t rvsrflags;
    u64_t rvsrsp;
    u64_t rvsss;
}stkparame_t;


typedef sysstus_t (*syscall_t)(uint_t inr,stkparame_t* stkparm);
#endif // KRLSERVICE_T_H
