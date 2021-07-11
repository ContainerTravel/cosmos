/**********************************************************
        线程调度文件krlsched.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void thrdlst_t_init(thrdlst_t *initp)
{
    list_init(&initp->tdl_lsth);
    initp->tdl_curruntd = NULL;
    initp->tdl_nr = 0;
    return;
}

void schdata_t_init(schdata_t *initp)
{
    krlspinlock_init(&initp->sda_lock);
    initp->sda_cpuid = hal_retn_cpuid();
    initp->sda_schdflgs = NOTS_SCHED_FLGS;
    initp->sda_premptidx = 0;
    initp->sda_threadnr = 0;
    initp->sda_prityidx = 0;
    initp->sda_cpuidle = NULL;
    initp->sda_currtd = NULL;
    for (uint_t ti = 0; ti < PRITY_MAX; ti++)
    {
        thrdlst_t_init(&initp->sda_thdlst[ti]);
    }
    return;
}

void schedclass_t_init(schedclass_t *initp)
{
    krlspinlock_init(&initp->scls_lock);
    initp->scls_cpunr = CPUCORE_MAX;
    initp->scls_threadnr = 0;
    initp->scls_threadid_inc = 0;
    for (uint_t si = 0; si < CPUCORE_MAX; si++)
    {
        schdata_t_init(&initp->scls_schda[si]);
    }
    return;
}

void init_krlsched()
{
    schedclass_t_init(&osschedcls);
    return;
}

thread_t *krlsched_retn_currthread()
{
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    if (schdap->sda_currtd == NULL)
    {
        hal_sysdie("schdap->sda_currtd NULL");
    }
    return schdap->sda_currtd;
}

uint_t krlsched_retn_schedflgs()
{
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    return schdap->sda_schdflgs;
}

void krlsched_wait(kwlst_t *wlst)
{
    cpuflg_t cufg, tcufg;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    thread_t *tdp = krlsched_retn_currthread();
    uint_t pity = tdp->td_priority;

    if (pity >= PRITY_MAX || wlst == NULL)
    {
        goto err_step;
    }
    if (schdap->sda_thdlst[pity].tdl_nr < 1)
    {
        goto err_step;
    }

    krlspinlock_cli(&schdap->sda_lock, &cufg);

    krlspinlock_cli(&tdp->td_lock, &tcufg);
    tdp->td_stus = TDSTUS_WAIT;
    list_del(&tdp->td_list);
    krlspinunlock_sti(&tdp->td_lock, &tcufg);

    if (schdap->sda_thdlst[pity].tdl_curruntd == tdp)
    {
        schdap->sda_thdlst[pity].tdl_curruntd = NULL;
    }
    schdap->sda_thdlst[pity].tdl_nr--;

    krlspinunlock_sti(&schdap->sda_lock, &cufg);
    krlwlst_add_thread(wlst, tdp);

    return;

err_step:
    hal_sysdie("krlsched_wait err");
    return;
}

void krlsched_up(kwlst_t *wlst)
{
    cpuflg_t cufg, tcufg;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    thread_t *tdp;
    uint_t pity;
    if (wlst == NULL)
    {
        goto err_step;
    }
    tdp = krlwlst_del_thread(wlst);
    if (tdp == NULL)
    {
        goto err_step;
    }
    pity = tdp->td_priority;
    if (pity >= PRITY_MAX)
    {
        goto err_step;
    }
    krlspinlock_cli(&schdap->sda_lock, &cufg);
    krlspinlock_cli(&tdp->td_lock, &tcufg);
    tdp->td_stus = TDSTUS_RUN;
    krlspinunlock_sti(&tdp->td_lock, &tcufg);
    list_add_tail(&tdp->td_list, &(schdap->sda_thdlst[pity].tdl_lsth));
    schdap->sda_thdlst[pity].tdl_nr++;
    krlspinunlock_sti(&schdap->sda_lock, &cufg);

    return;
err_step:
    hal_sysdie("krlsched_up err");
    return;
}

thread_t *krlsched_retn_idlethread()
{
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    if (schdap->sda_cpuidle == NULL)
    {
        hal_sysdie("schdap->sda_cpuidle NULL");
    }
    return schdap->sda_cpuidle;
}

void krlsched_set_schedflgs()
{
    cpuflg_t cpuflg;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    krlspinlock_cli(&schdap->sda_lock, &cpuflg);
    schdap->sda_schdflgs = NEED_SCHED_FLGS;
    krlspinunlock_sti(&schdap->sda_lock, &cpuflg);
    return;
}

void krlsched_set_schedflgs_ex(uint_t flags)
{
    cpuflg_t cpuflg;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    krlspinlock_cli(&schdap->sda_lock, &cpuflg);
    schdap->sda_schdflgs = flags;
    krlspinunlock_sti(&schdap->sda_lock, &cpuflg);
    return;
}

void krlsched_chkneed_pmptsched()
{
    cpuflg_t cpuflg;
    uint_t schd = 0, cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    krlspinlock_cli(&schdap->sda_lock, &cpuflg);

    if (schdap->sda_schdflgs == NEED_SCHED_FLGS && schdap->sda_premptidx == PMPT_FLGS)
    {
        schdap->sda_schdflgs = NOTS_SCHED_FLGS;
        schd = 1;
    }
    if (schdap->sda_schdflgs == NEED_START_CPUILDE_SCHED_FLGS)
    {
        schd = 1;
    }
    krlspinunlock_sti(&schdap->sda_lock, &cpuflg);
    if (schd == 1)
    {

        krlschedul();
    }
    return;
}
thread_t *krlsched_select_thread()
{
    thread_t *retthd, *tdtmp;
    cpuflg_t cufg;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    krlspinlock_cli(&schdap->sda_lock, &cufg);
    for (uint_t pity = 0; pity < PRITY_MAX; pity++)
    {
        if (schdap->sda_thdlst[pity].tdl_nr > 0)
        {
            if (list_is_empty_careful(&(schdap->sda_thdlst[pity].tdl_lsth)) == FALSE)
            {
                tdtmp = list_entry(schdap->sda_thdlst[pity].tdl_lsth.next, thread_t, td_list);
                list_del(&tdtmp->td_list);
                if (schdap->sda_thdlst[pity].tdl_curruntd != NULL)
                {
                    list_add_tail(&(schdap->sda_thdlst[pity].tdl_curruntd->td_list), &schdap->sda_thdlst[pity].tdl_lsth);
                }
                schdap->sda_thdlst[pity].tdl_curruntd = tdtmp;
                retthd = tdtmp;

                goto return_step;
            }
            if (schdap->sda_thdlst[pity].tdl_curruntd != NULL)
            {
                retthd = schdap->sda_thdlst[pity].tdl_curruntd;
                goto return_step;
            }
        }
    }

    schdap->sda_prityidx = PRITY_MIN;
    retthd = krlsched_retn_idlethread();

return_step:

    krlspinunlock_sti(&schdap->sda_lock, &cufg);
    return retthd;
}

void krlschedul()
{

    if (krlsched_retn_schedflgs() == NEED_START_CPUILDE_SCHED_FLGS)
    {
        krlsched_set_schedflgs_ex(NOTS_SCHED_FLGS);
        retnfrom_first_sched(krlsched_retn_idlethread());
        return;
    }
    thread_t *prev = krlsched_retn_currthread(),
             *next = krlsched_select_thread();
    kprint("调度器运行 当前进程:%x,下一个进程:%x\n", prev, next);
    save_to_new_context(next, prev);
    return;
}

void krlschdclass_add_thread(thread_t *thdp)
{
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    cpuflg_t cufg;

    krlspinlock_cli(&schdap->sda_lock, &cufg);
    list_add(&thdp->td_list, &schdap->sda_thdlst[thdp->td_priority].tdl_lsth);
    schdap->sda_thdlst[thdp->td_priority].tdl_nr++;
    schdap->sda_threadnr++;
    krlspinunlock_sti(&schdap->sda_lock, &cufg);

    krlspinlock_cli(&osschedcls.scls_lock, &cufg);
    osschedcls.scls_threadnr++;
    krlspinunlock_sti(&osschedcls.scls_lock, &cufg);

    return;
}

void __to_new_context(thread_t *next, thread_t *prev)
{
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    schdap->sda_currtd = next;

    next->td_context.ctx_nexttss = &x64tss[cpuid];
    next->td_context.ctx_nexttss->rsp0 = next->td_krlstktop;
    hal_mmu_load(&next->td_mmdsc->msd_mmu);
    if (next->td_stus == TDSTUS_NEW)
    {
        next->td_stus = TDSTUS_RUN;

        retnfrom_first_sched(next);
    }

    return;
}

void save_to_new_context(thread_t *next, thread_t *prev)
{
#ifdef CFG_X86_PLATFORM
    __asm__ __volatile__(
        "pushfq \n\t"
        "cli \n\t"
        "pushq %%rax\n\t"
        "pushq %%rbx\n\t"
        "pushq %%rcx\n\t"
        "pushq %%rdx\n\t"
        "pushq %%rbp\n\t"
        "pushq %%rsi\n\t"
        "pushq %%rdi\n\t"
        "pushq %%r8\n\t"
        "pushq %%r9\n\t"
        "pushq %%r10\n\t"
        "pushq %%r11\n\t"
        "pushq %%r12\n\t"
        "pushq %%r13\n\t"
        "pushq %%r14\n\t"
        "pushq %%r15\n\t"

        "movq %%rsp,%[PREV_RSP] \n\t"
        "movq %[NEXT_RSP],%%rsp \n\t"
        "callq __to_new_context\n\t"

        "popq %%r15\n\t"
        "popq %%r14\n\t"
        "popq %%r13\n\t"
        "popq %%r12\n\t"
        "popq %%r11\n\t"
        "popq %%r10\n\t"
        "popq %%r9\n\t"
        "popq %%r8\n\t"
        "popq %%rdi\n\t"
        "popq %%rsi\n\t"
        "popq %%rbp\n\t"
        "popq %%rdx\n\t"
        "popq %%rcx\n\t"
        "popq %%rbx\n\t"
        "popq %%rax\n\t"
        "popfq \n\t"
        : [ PREV_RSP ] "=m"(prev->td_context.ctx_nextrsp)
        : [ NEXT_RSP ] "m"(next->td_context.ctx_nextrsp), "D"(next), "S"(prev)
        : "memory");
#endif
    return;
}

void retnfrom_first_sched(thread_t *thrdp)
{

#ifdef CFG_X86_PLATFORM
    __asm__ __volatile__(
        "movq %[NEXT_RSP],%%rsp\n\t"
        "popq %%r14\n\t"
        "movw %%r14w,%%gs\n\t"
        "popq %%r14\n\t"
        "movw %%r14w,%%fs\n\t"
        "popq %%r14\n\t"
        "movw %%r14w,%%es\n\t"
        "popq %%r14\n\t"
        "movw %%r14w,%%ds\n\t"
        "popq %%r15\n\t"
        "popq %%r14\n\t"
        "popq %%r13\n\t"
        "popq %%r12\n\t"
        "popq %%r11\n\t"
        "popq %%r10\n\t"
        "popq %%r9\n\t"
        "popq %%r8\n\t"
        "popq %%rdi\n\t"
        "popq %%rsi\n\t"
        "popq %%rbp\n\t"
        "popq %%rdx\n\t"
        "popq %%rcx\n\t"
        "popq %%rbx\n\t"
        "popq %%rax\n\t"
        "iretq\n\t"

        :
        : [ NEXT_RSP ] "m"(thrdp->td_context.ctx_nextrsp)
        : "memory");
#endif
}
