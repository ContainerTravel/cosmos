/**********************************************************
        idle线程头文件krlcpuidle.c
***********************************************************
                彭东 
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void init_krlcpuidle()
{
    new_cpuidle();
    krlcpuidle_start();
    return;
}

void krlcpuidle_start()
{

    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    thread_t *tdp = schdap->sda_cpuidle;
    kprint("schdap->sda_cpuidle:%x\n\r", schdap->sda_cpuidle);

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
    size_t usrstksz = DAFT_TDUSRSTKSZ, krlstksz = DAFT_TDKRLSTKSZ;
    adr_t usrstkadr = NULL, krlstkadr = NULL;
    usrstkadr = krlnew(usrstksz);
    if (usrstkadr == NULL)
    {
        return NULL;
    }
    krlstkadr = krlnew(krlstksz);
    if (krlstkadr == NULL)
    {
        if (krldelete(usrstkadr, usrstksz) == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    ret_td = krlnew_thread_dsc();
    if (ret_td == NULL)
    {
        acs = krldelete(usrstkadr, usrstksz);
        acs = krldelete(krlstkadr, krlstksz);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    ret_td->td_privilege = PRILG_USR;
    ret_td->td_priority = PRITY_MAX;

    ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
    ret_td->td_krlstkstart = krlstkadr;
    ret_td->td_usrstktop = usrstkadr + (adr_t)(usrstksz - 1);
    ret_td->td_usrstkstart = usrstkadr;
    krlthread_kernstack_init(ret_td, (void *)krlcpuidle_main, KMOD_EFLAGS);
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
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
        //hal_sysdie("cpuidle DIE\n");
        kprint("cpuidle is run:%x\n", i);
        krlschedul();
    }
    return;
}
