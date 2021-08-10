/**********************************************************
        hal层中断处理头文件halintupt.c
***********************************************************
                彭东
**********************************************************/

#include "cosmostypes.h"
#include "cosmosmctrl.h"

void dump_stack(void* stk)
{
    faultstkregs_t* f = (faultstkregs_t*)stk;
    kprint("程序指令寄存器%x:\n",f->r_rip_old);
    kprint("程序标志寄存器%x:\n",f->r_rflgs);
    kprint("程序栈寄存器%x:\n",f->r_rsp_old);
    kprint("程序缺页地址%x:\n",read_cr2());
    kprint("程序段寄存器CS:%x SS:%x DS:%x ES:%x FS:%x GS:%x\n",
    f->r_cs_old, f->r_ss_old, f->r_ds, f->r_es, f->r_fs, f->r_gs);
    return;
}

void intfltdsc_t_init(intfltdsc_t *initp, u32_t flg, u32_t sts, uint_t prity, uint_t irq)
{
    hal_spinlock_init(&initp->i_lock);
    initp->i_flg = flg;
    initp->i_stus = sts;
    initp->i_prity = prity;
    initp->i_irqnr = irq;
    initp->i_deep = 0;
    initp->i_indx = 0;
    list_init(&initp->i_serlist);
    initp->i_sernr = 0;
    list_init(&initp->i_serthrdlst);
    initp->i_serthrdnr = 0;
    initp->i_onethread = NULL;
    initp->i_rbtreeroot = NULL;
    list_init(&initp->i_serfisrlst);
    initp->i_serfisrnr = 0;
    initp->i_msgmpool = NULL;
    initp->i_privp = NULL;
    initp->i_extp = NULL;
    return;
}

void init_intfltdsc()
{
    for (uint_t i = 0; i < IDTMAX; i++)
    {
        intfltdsc_t_init(&machintflt[i], 0, 0, i, i);
    }
    return;
}

PUBLIC void init_halintupt()
{
    init_descriptor();
    init_idt_descriptor();
    init_intfltdsc();
    init_i8259();
    i8259_enabled_line(0);
    kprint("中断初始化成功\n");
    // die(0x400);
    return;
}

PUBLIC intfltdsc_t *hal_retn_intfltdsc(uint_t irqnr)
{
    if (irqnr > IDTMAX)
    {
        return NULL;
    }
    return &machintflt[irqnr];
}

void intserdsc_t_init(intserdsc_t *initp, u32_t flg, intfltdsc_t *intfltp, void *device, intflthandle_t handle)
{

    list_init(&initp->s_list);
    list_init(&initp->s_indevlst);
    initp->s_flg = flg;
    initp->s_intfltp = intfltp;
    initp->s_indx = 0;
    initp->s_device = device;
    initp->s_handle = handle;
    return;
}

bool_t hal_add_ihandle(intfltdsc_t *intdscp, intserdsc_t *serdscp)
{
    if (intdscp == NULL || serdscp == NULL)
    {
        return FALSE;
    }
    cpuflg_t cpuflg;
    hal_spinlock_saveflg_cli(&intdscp->i_lock, &cpuflg);
    list_add(&serdscp->s_list, &intdscp->i_serlist);
    intdscp->i_sernr++;
    hal_spinunlock_restflg_sti(&intdscp->i_lock, &cpuflg);
    return TRUE;
}

drvstus_t hal_enable_intline(uint_t ifdnr)
{
    if (20 > ifdnr || 36 < ifdnr)
    {
        return DFCERRSTUS;
    }
    i8259_enabled_line((u32_t)ifdnr);
    return DFCOKSTUS;
}

drvstus_t hal_disable_intline(uint_t ifdnr)
{

    if (20 > ifdnr || 36 < ifdnr)
    {
        return DFCERRSTUS;
    }
    i8259_disable_line((u32_t)ifdnr);
    return DFCOKSTUS;
}

drvstus_t hal_intflt_default(uint_t ift_nr, void *sframe)
{
    if (ift_nr == 0xffffffff || sframe == NULL)
    {
        return DFCERRSTUS;
    }
    return DFCOKSTUS;
}

void hal_run_intflthandle(uint_t ifdnr, void *sframe)
{
    intserdsc_t *isdscp;
    list_h_t *lst;
    intfltdsc_t *ifdscp = hal_retn_intfltdsc(ifdnr);
    if (ifdscp == NULL)
    {
        hal_sysdie("hal_run_intfdsc err");
        return;
    }

    list_for_each(lst, &ifdscp->i_serlist)
    {
        isdscp = list_entry(lst, intserdsc_t, s_list);
        isdscp->s_handle(ifdnr, isdscp->s_device, sframe);
    }

    return;
}
void hal_hwint_eoi()
{
    i8259_send_eoi();
    return;
}
void hal_do_hwint(uint_t intnumb, void *krnlsframp)
{
    intfltdsc_t *ifdscp = NULL;
    cpuflg_t cpuflg;
    if (intnumb > IDTMAX || krnlsframp == NULL)
    {
        hal_sysdie("hal_do_hwint fail\n");
        return;
    }
    ifdscp = hal_retn_intfltdsc(intnumb);
    if (ifdscp == NULL)
    {
        hal_sysdie("hal_do_hwint ifdscp NULL\n");
        return;
    }
    hal_spinlock_saveflg_cli(&ifdscp->i_lock, &cpuflg);
    ifdscp->i_indx++;
    ifdscp->i_deep++;
    hal_run_intflthandle(intnumb, krnlsframp);
    ifdscp->i_deep--;
    hal_spinunlock_restflg_sti(&ifdscp->i_lock, &cpuflg);
    return;
}

void hal_fault_allocator(uint_t faultnumb, void *krnlsframp) //eax,edx
{
    adr_t fairvadrs;
    cpuflg_t cpuflg;
    hal_cpuflag_sti(&cpuflg);
    if (faultnumb == 14)
    {
        fairvadrs = (adr_t)read_cr2();
        // kprint("当前进程:%s,缺页异常地址:%x\n", krlsched_retn_currthread()->td_appfilenm, fairvadrs);
        if (krluserspace_accessfailed(fairvadrs) != 0)
        {
            dump_stack(krnlsframp);
            system_error("缺页处理失败\n");
        }
        hal_cpuflag_cli(&cpuflg);
        return;
    }
    kprint("当前进程:%s,犯了不该犯的错误:%d,所以要杀\n", krlsched_retn_currthread()->td_appfilenm, faultnumb);
    dump_stack(krnlsframp);
    krlsve_exit_thread();
    hal_cpuflag_cli(&cpuflg);
    return;
}

sysstus_t hal_syscl_allocator(uint_t inr,void* krnlsframp)
{
    cpuflg_t cpuflg;
    sysstus_t ret;
    hal_cpuflag_sti(&cpuflg);
	ret = krlservice(inr,krnlsframp);
    hal_cpuflag_cli(&cpuflg);
    return ret;
}

void hal_hwint_allocator(uint_t intnumb, void *krnlsframp)
{
    cpuflg_t cpuflg;
    hal_cpuflag_sti(&cpuflg);
    hal_hwint_eoi();
    hal_do_hwint(intnumb, krnlsframp);   
    krlsched_chkneed_pmptsched();
    hal_cpuflag_cli(&cpuflg);
    return;
}
