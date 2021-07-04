/****************************************************************
        kernel全局数据结构文件krlglobal.c
*****************************************************************
                彭东 
****************************************************************/
#define KRLGOBAL_HEAD
#include "cosmostypes.h"
#include "cosmosmctrl.h"

KRL_DEFGLOB_VARIABLE(kvirmemadrs_t, krlvirmemadrs);
KRL_DEFGLOB_VARIABLE(mmadrsdsc_t, initmmadrsdsc);
KRL_DEFGLOB_VARIABLE(kmempool_t,oskmempool);
KRL_DEFGLOB_VARIABLE(schedclass_t,osschedcls);
// KRL_DEFGLOB_VARIABLE(ktime_t,osktime);
// KRL_DEFGLOB_VARIABLE(syscall_t,osscalltab)[SWINR_MAX]={
// NULL,krlsvetabl_mallocblk,
// krlsvetabl_mfreeblk,krlsvetabl_exel_thread,
// krlsvetabl_exit_thread,
// krlsvetabl_retn_threadhand,
// krlsvetabl_retn_threadstats,
// krlsvetabl_set_threadstats,
// krlsvetabl_open,krlsvetabl_close,
// krlsvetabl_read,krlsvetabl_write,
// krlsvetabl_ioctrl,krlsvetabl_lseek,
// krlsvetabl_time};
// KRL_DEFGLOB_VARIABLE(devtable_t,osdevtable);
// KRL_DEFGLOB_VARIABLE(iocheblkdsc_t,osiocheblk);
// KRL_DEFGLOB_VARIABLE(drventyexit_t,osdrvetytabl)[]={
// systick_entry,/*uart_entry,rtc_entry,rfs_entry,*/
// NULL};
