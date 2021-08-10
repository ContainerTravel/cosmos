/**********************************************************
        线程管理文件krlthread.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void micrstk_t_init(micrstk_t *initp)
{
    for (uint_t i = 0; i < MICRSTK_MAX; i++)
    {
        initp->msk_val[i] = 0;
    }
    return;
}

void context_t_init(context_t *initp)
{

    initp->ctx_nextrip = 0;
    initp->ctx_nextrsp = 0;
    initp->ctx_nexttss = &x64tss[hal_retn_cpuid()];
    return;
}

uint_t krlretn_thread_id(thread_t *tdp)
{
    return (uint_t)tdp;
}
void thread_t_init(thread_t *initp)
{
    krlspinlock_init(&initp->td_lock);
    list_init(&initp->td_list);
    initp->td_flgs = TDFLAG_FREE;
    initp->td_stus = TDSTUS_NEW;
    initp->td_cpuid = hal_retn_cpuid();
    initp->td_id = krlretn_thread_id(initp);
    initp->td_tick = 0;
    initp->td_sumtick = 0;
    initp->td_privilege = PRILG_USR;
    initp->td_priority = PRITY_MIN;
    initp->td_runmode = 0;
    initp->td_krlstktop = NULL;
    initp->td_krlstkstart = NULL;
    initp->td_usrstktop = NULL;
    initp->td_usrstkstart = NULL;
    initp->td_mmdsc = &initmmadrsdsc;
    initp->td_resdsc = NULL;
    initp->td_privtep = NULL;
    initp->td_extdatap = NULL;
    initp->td_appfilenm = NULL;
    initp->td_appfilenmlen = 0;
    context_t_init(&initp->td_context);
    for (uint_t hand = 0; hand < TD_HAND_MAX; hand++)
    {
        initp->td_handtbl[hand] = NULL;
    }
    krlmemset((void*)initp->td_name, 0, THREAD_NAME_MAX);
    return;
}

thread_t *krlnew_thread_dsc()
{

    thread_t *rettdp = (thread_t *)(krlnew((size_t)(sizeof(thread_t))));
    if (rettdp == NULL)
    {
        return NULL;
    }
    thread_t_init(rettdp);
    return rettdp;
}

void krlthd_inc_tick(thread_t *thdp)
{

    cpuflg_t cpuflg;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);
    thdp->td_tick++;
    thdp->td_sumtick++;

    if (thdp->td_tick > TDRUN_TICK)
    {
        thdp->td_tick = 0;
        krlsched_set_schedflgs();
    }
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return;
}

uint_t krlthread_sumtick(thread_t* thdp)
{
    if(NULL == thdp)
    {
        return 0;
    }
    return thdp->td_sumtick;
}

uint_t krlthd_curr_sumtick()
{
    return krlthread_sumtick(krlsched_retn_currthread());
}

hand_t krlthd_retn_nullhand(thread_t *thdp)
{
    cpuflg_t cpuflg;
    hand_t rethd = NO_HAND;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);
    for (uint_t hand = 0; hand < TD_HAND_MAX; hand++)
    {
        if (thdp->td_handtbl[hand] == NULL)
        {
            rethd = (hand_t)hand;
            goto retn_step;
        }
    }
    rethd = NO_HAND;
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return rethd;
}

hand_t krlthd_add_objnode(thread_t *thdp, objnode_t *ondp)
{
    cpuflg_t cpuflg;
    hand_t rethd = NO_HAND;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);
    for (uint_t hand = 0; hand < TD_HAND_MAX; hand++)
    {
        if (thdp->td_handtbl[hand] == NULL)
        {
            rethd = (hand_t)hand;
            goto next_step;
        }
    }
    rethd = NO_HAND;
    goto retn_step;

next_step:
    thdp->td_handtbl[rethd] = ondp;
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return rethd;
}

hand_t krlthd_del_objnode(thread_t *thdp, hand_t hand)
{
    if ((hand >= TD_HAND_MAX) || (hand <= NO_HAND))
    {
        return NO_HAND;
    }

    cpuflg_t cpuflg;
    hand_t rethd = NO_HAND;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);

    if (thdp->td_handtbl[hand] == NULL)
    {
        rethd = NO_HAND;
        goto retn_step;
    }
    thdp->td_handtbl[hand] = NULL;
    rethd = hand;
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return rethd;
}

objnode_t *krlthd_retn_objnode(thread_t *thdp, hand_t hand)
{
    if ((hand >= TD_HAND_MAX) || (hand <= NO_HAND))
    {
        return NULL;
    }

    cpuflg_t cpuflg;
    objnode_t *retondp = NULL;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);

    if (thdp->td_handtbl[hand] == NULL)
    {
        retondp = NULL;
        goto retn_step;
    }
    retondp = thdp->td_handtbl[hand];
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return retondp;
}

void krlthread_kernstack_init(thread_t *thdp, void *runadr, uint_t cpuflags)
{
    thdp->td_krlstktop &= (~0xf);
    thdp->td_usrstktop &= (~0xf);
    intstkregs_t *arp = (intstkregs_t *)(thdp->td_krlstktop - sizeof(intstkregs_t));
    hal_memset((void*)arp, 0, sizeof(intstkregs_t));
    
    arp->r_rip_old = (uint_t)runadr;
    arp->r_cs_old = K_CS_IDX;
    arp->r_rflgs = cpuflags;
    arp->r_rsp_old = thdp->td_krlstktop;
    arp->r_ss_old = 0;
   
    arp->r_ds = K_DS_IDX;
    arp->r_es = K_DS_IDX;
    arp->r_fs = K_DS_IDX;
    arp->r_gs = K_DS_IDX;
 
    thdp->td_context.ctx_nextrip = (uint_t)runadr;
    thdp->td_context.ctx_nextrsp = (uint_t)arp;

    return;
}

void krlthread_userstack_init(thread_t *thdp, void *runadr, uint_t cpuflags)
{
    thdp->td_krlstktop &= (~0xf);
    thdp->td_usrstktop &= (~0xf);
    intstkregs_t *arp = (intstkregs_t *)(thdp->td_krlstktop - sizeof(intstkregs_t));
    hal_memset((void*)arp, 0, sizeof(intstkregs_t));
    
    arp->r_rip_old = (uint_t)runadr;
    arp->r_cs_old = U_CS_IDX;
    arp->r_rflgs = cpuflags;
    arp->r_rsp_old = thdp->td_usrstktop;
    arp->r_ss_old = U_DS_IDX;
   
    arp->r_ds = U_DS_IDX;
    arp->r_es = U_DS_IDX;
    arp->r_fs = U_DS_IDX;
    arp->r_gs = U_DS_IDX;
 
    thdp->td_context.ctx_nextrip = (uint_t)runadr;
    thdp->td_context.ctx_nextrsp = (uint_t)arp;

    return;
}

char_t* thread_name(thread_t* thread, char_t* name)
{
    uint_t len = 0;
    adr_t vadr = NULL;
    if(NULL == thread)
    {
        return NULL;
    }
    if(NULL == name)
    {
        krlstrcpy("unkown thread", thread->td_name);
        thread->td_appfilenm = thread->td_name;
        thread->td_appfilenmlen = THREAD_NAME_MAX;
        return thread->td_appfilenm;
    }
    len = krlstrlen(name);
    if(len < (THREAD_NAME_MAX - 1))
    {
        krlstrcpy(name, thread->td_name);
        thread->td_appfilenm = thread->td_name;
        thread->td_appfilenmlen = THREAD_NAME_MAX;
        return thread->td_appfilenm;
    }
    vadr = krlnew(((size_t)len + 1));
    if(NULL == vadr)
    {
        return NULL;
    }
    krlmemset((void*)vadr, 0, len + 1);
    krlstrcpy(name, (char_t*)vadr);
    thread->td_appfilenm = (char_t*)vadr;
    thread->td_appfilenmlen = (uint_t)(len + 1);    
    return thread->td_appfilenm;
}

thread_t *krlnew_user_thread_core(char_t* name, void *filerun, uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz, size_t krlstksz)
{
    thread_t *ret_td = NULL;
    bool_t acs = FALSE;
    adr_t usrstkadr = NULL, usrstktop = NULL, krlstkadr = NULL;
    mmadrsdsc_t* mm; 
    mm = new_mmadrsdsc();
    if(NULL == mm)
    {
        return NULL;
    }
    if(NULL == mm->msd_virmemadrs.vs_stackkmvdsc)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }

    usrstkadr = mm->msd_virmemadrs.vs_stackkmvdsc->kva_start;
    usrstktop = mm->msd_virmemadrs.vs_stackkmvdsc->kva_end;
    if(USER_VIRTUAL_ADDRESS_END != usrstktop)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }

    if((usrstktop - usrstkadr) < usrstksz)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }
  
    krlstkadr = krlnew(krlstksz);
    if (krlstkadr == NULL)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }
    ret_td = krlnew_thread_dsc();
    if (ret_td == NULL)
    {
        acs = krldelete(krlstkadr, krlstksz);
        acs = del_mmadrsdsc(mm);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    thread_name(ret_td, name);
    ret_td->td_mmdsc = mm;
    mm->msd_thread = ret_td;
    ret_td->td_privilege = prilg;
    ret_td->td_priority = prity;

    ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
    ret_td->td_krlstkstart = krlstkadr;
    ret_td->td_usrstktop = usrstktop;
    ret_td->td_usrstkstart = usrstkadr;

    krlthread_userstack_init(ret_td, filerun, UMOD_EFLAGS);

    krlschdclass_add_thread(ret_td);
    return ret_td;
}

thread_t *krlnew_kern_thread_core(char_t* name, void *filerun, uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz, size_t krlstksz)
{
    thread_t *ret_td = NULL;
    bool_t acs = FALSE;
    adr_t krlstkadr = NULL;

    krlstkadr = krlnew(krlstksz);
    if (krlstkadr == NULL)
    {
        return NULL;
    }
    ret_td = krlnew_thread_dsc();
    if (ret_td == NULL)
    {
        acs = krldelete(krlstkadr, krlstksz);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    thread_name(ret_td, name);
    ret_td->td_privilege = prilg;
    ret_td->td_priority = prity;

    ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
    ret_td->td_krlstkstart = krlstkadr;


    krlthread_kernstack_init(ret_td, filerun, KMOD_EFLAGS);

    krlschdclass_add_thread(ret_td);
    return ret_td;
}

thread_t *krlnew_thread(char_t* name, void *filerun, uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz, size_t krlstksz)
{
    size_t tustksz = usrstksz, tkstksz = krlstksz;
    if (filerun == NULL || usrstksz > DAFT_TDUSRSTKSZ || krlstksz > DAFT_TDKRLSTKSZ)
    {
        return NULL;
    }

    if ((prilg != PRILG_USR && prilg != PRILG_SYS) || (prity >= PRITY_MAX))
    {
        return NULL;
    }
    if (usrstksz < DAFT_TDUSRSTKSZ)
    {
        tustksz = DAFT_TDUSRSTKSZ;
    }
    if (krlstksz < DAFT_TDKRLSTKSZ)
    {
        tkstksz = DAFT_TDKRLSTKSZ;
    }
    if (KERNTHREAD_FLG == flg)
    {
        return krlnew_kern_thread_core(name, filerun, flg, prilg, prity, tustksz, tkstksz);
    }
    else if (USERTHREAD_FLG == flg)
    {
        return krlnew_user_thread_core(name, filerun, flg, prilg, prity, tustksz, tkstksz);
    }
    return NULL;
}

thread_t* krlthread_execvl(thread_t* thread, char_t* filename)
{
    u64_t retadr = 0, filelen = 0;
    adr_t vadr = 0;
    if(NULL == thread || NULL == filename)
    {
        return NULL;
    }
    get_file_rvadrandsz(filename, &kmachbsp, &retadr, &filelen);
    if(NULL == retadr || 0 == filelen)
    {
        return NULL;
    }
    if(thread->td_mmdsc == &initmmadrsdsc)
    {
        return NULL;
    }
    if((0x100000+filelen) >= THREAD_HEAPADR_START)
    {
        return NULL;
    }
    vadr = kvma_initdefault_virmemadrs(thread->td_mmdsc, APPRUN_START_VITRUALADDR, filelen, KMV_BIN_TYPE);
    if(NULL == vadr)
    {
        return NULL;
    }
    
    return thread;

}