/**********************************************************
        idle线程头文件krlcpuidle.c
***********************************************************
                彭东 
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void thread_a_main()
{
    uint_t i = 0;
    for (;; i++)
    {
        kprint("进程A运行:%x\n", i);
        krlschedul();
    }
    return;
}

void thread_b_main()
{
    uint_t i = 0;
    for (;; i++)
    {
        kprint("进程B运行:%x\n", i);
        krlschedul();
    }
    return;
}

void init_ab_thread()
{
    krlnew_thread((void*)thread_a_main, KERNTHREAD_FLG, 
                PRILG_SYS, PRITY_MIN, DAFT_TDUSRSTKSZ, DAFT_TDKRLSTKSZ);
    krlnew_thread((void*)thread_b_main, KERNTHREAD_FLG, 
                PRILG_SYS, PRITY_MIN, DAFT_TDUSRSTKSZ, DAFT_TDKRLSTKSZ);
    return;
}

void init_krlcpuidle()
{
    new_cpuidle();
    init_ab_thread();
    krlcpuidle_start();
    return;
}

void krlcpuidle_start()
{

    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    thread_t *tdp = schdap->sda_cpuidle;
    tdp->td_context.ctx_nexttss = &x64tss[cpuid];
    tdp->td_context.ctx_nexttss->rsp0 = tdp->td_krlstktop;
    tdp->td_stus = TDSTUS_RUN;
    retnfrom_first_sched(tdp);

    return;
}
thread_t *new_cpuidle_thread()
{

    thread_t *ret_td = NULL;
    bool_t acs = FALSE;
    adr_t krlstkadr = NULL;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    krlstkadr = krlnew(DAFT_TDKRLSTKSZ);
    if (krlstkadr == NULL)
    {
        return NULL;
    }
    ret_td = krlnew_thread_dsc();
    if (ret_td == NULL)
    {
        acs = krldelete(krlstkadr, DAFT_TDKRLSTKSZ);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    ret_td->td_privilege = PRILG_SYS;
    ret_td->td_priority = PRITY_MIN;

    ret_td->td_krlstktop = krlstkadr + (adr_t)(DAFT_TDKRLSTKSZ - 1);
    ret_td->td_krlstkstart = krlstkadr;
    krlthread_kernstack_init(ret_td, (void *)krlcpuidle_main, KMOD_EFLAGS);
   
    schdap->sda_cpuidle = ret_td;
    schdap->sda_currtd = ret_td;

    return ret_td;
}

void new_cpuidle()
{
    thread_t *thp = new_cpuidle_thread();
    if (thp == NULL)
    {
        hal_sysdie("newcpuilde err");
    }
    kprint("CPUIDLETASK: %x\n", (uint_t)thp);
    return;
}

void krlcpuidle_main()
{
    uint_t i = 0;
    for (;; i++)
    {
        kprint("空转进程运行:%x\n", i);
        krlschedul();
    }
    return;
}
