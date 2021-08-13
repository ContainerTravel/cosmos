/**********************************************************
        线程调度头文件krlsched_t.h
***********************************************************
                彭东 
**********************************************************/
#ifndef _KRLSCHED_T_H
#define _KRLSCHED_T_H
#define NOTS_SCHED_FLGS (0)
#define NEED_SCHED_FLGS (1)
#define NEED_START_CPUILDE_SCHED_FLGS (2)
#define PMPT_FLGS 0

#ifdef CFG_X86_PLATFORM
#define TNCCALL __attribute__((regparm(2)))         
#endif
#if((defined CFG_S3C2440A_PLATFORM) || (defined CFG_STM32F0XX_PLATFORM))
#define TNCCALL 
#endif

typedef struct s_THRDLST
{
    list_h_t    tdl_lsth;
    thread_t*   tdl_curruntd;
    uint_t      tdl_nr; 
}thrdlst_t;
typedef struct s_SCHDATA
{
    spinlock_t  sda_lock;
    uint_t      sda_cpuid;
    uint_t      sda_schdflgs;
    uint_t      sda_premptidx;
    uint_t      sda_threadnr;
    uint_t      sda_prityidx;
    thread_t*   sda_cpuidle;
    thread_t*   sda_currtd;
    thrdlst_t   sda_thdlst[PRITY_MAX];
    list_h_t    sda_exitlist;  
}schdata_t;

typedef struct s_SCHEDCALSS
{
    spinlock_t  scls_lock;
    uint_t      scls_cpunr;
    uint_t      scls_threadnr;
    uint_t      scls_threadid_inc;
    schdata_t   scls_schda[CPUCORE_MAX];
}schedclass_t;



#endif // KRLSCHED_T_H
