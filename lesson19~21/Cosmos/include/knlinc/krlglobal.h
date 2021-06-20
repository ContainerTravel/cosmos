/****************************************************************
        kernel全局数据结构头文件krlglobal.h
*****************************************************************
                彭东
****************************************************************/
#ifndef _KRLGLOBAL_H
#define _KRLGLOBAL_H
#ifdef	KRLGOBAL_HEAD
#undef	KEXTERN
#define KEXTERN
#endif
#if((defined CFG_X86_PLATFORM) || (defined CFG_S3C2440A_PLATFORM))   
KRL_DEFGLOB_VARIABLE(kvirmemadrs_t, krlvirmemadrs);
KRL_DEFGLOB_VARIABLE(mmadrsdsc_t, initmmadrsdsc);

KRL_DEFGLOB_VARIABLE(kmempool_t,oskmempool);
// KRL_DEFGLOB_VARIABLE(schedclass_t,osschedcls);
// KRL_DEFGLOB_VARIABLE(ktime_t,osktime);
// KRL_DEFGLOB_VARIABLE(syscall_t,osscalltab)[SWINR_MAX];
// KRL_DEFGLOB_VARIABLE(devtable_t,osdevtable);
// KRL_DEFGLOB_VARIABLE(iocheblkdsc_t,osiocheblk);
// KRL_DEFGLOB_VARIABLE(drventyexit_t,osdrvetytabl)[];
#endif

#endif // KRLGLOBAL_H
