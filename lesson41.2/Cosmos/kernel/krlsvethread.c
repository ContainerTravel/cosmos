/**********************************************************
        内核服务文件krlsvethread.c
***********************************************************
                彭东 
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

sysstus_t krlsvetabl_tick(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TD_TICK)
    {
        return SYSSTUSERR;
    }
    return (sysstus_t)krlsve_thread_tick((uint_t)stkparv->parmv1);
}

uint_t krlsve_thread_tick(uint_t id)
{
    return krlthd_curr_sumtick();
}

sysstus_t krlsvetabl_exel_thread(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TD_EXEL)
    {
        return SYSSTUSERR;
    }
    return krlsve_exel_thread((void *)stkparv->parmv1, (uint_t)stkparv->parmv2);
}

sysstus_t krlsvetabl_exit_thread(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TD_EXIT)
    {
        return SYSSTUSERR;
    }
    return krlsve_exit_thread();
}

sysstus_t krlsvetabl_retn_threadhand(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TD_HAND)
    {
        return SYSSTUSERR;
    }
    return (sysstus_t)krlsve_retn_threadhand((void *)stkparv->parmv1);
}

sysstus_t krlsvetabl_retn_threadstats(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TD_RSTATS)
    {
        return SYSSTUSERR;
    }
    return krlsve_retn_threadstats((hand_t)stkparv->parmv1, (uint_t)stkparv->parmv2,
                                   (uint_t)stkparv->parmv3, (buf_t)stkparv->parmv4);
}

sysstus_t krlsvetabl_set_threadstats(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TD_SSTATS)
    {
        return SYSSTUSERR;
    }
    return krlsve_set_threadstats((hand_t)stkparv->parmv1, (uint_t)stkparv->parmv2,
                                  (uint_t)stkparv->parmv3, (buf_t)stkparv->parmv4);
}

hand_t krlsve_exel_thread(void *file, uint_t flgs)
{
    return krlsve_core_exel_thread(file, flgs);
}

sysstus_t krlsve_exit_thread()
{
    krlsve_core_exit_thread();
    return SYSSTUSOK;
}


hand_t krlsve_retn_threadhand(void *tname)
{
    if (tname == NULL)
    {
        thread_t *tdp = krlsched_retn_currthread();
        return (hand_t)(tdp->td_id);
    }
    return SYSSTUSERR;
}

sysstus_t krlsve_retn_threadstats(hand_t thand, uint_t scode, uint_t data, buf_t buf)
{
    return SYSSTUSERR;
}

sysstus_t krlsve_set_threadstats(hand_t thand, uint_t scode, uint_t data, buf_t buf)
{

    return SYSSTUSERR;
}

hand_t krlsve_core_exel_thread(void *file, uint_t flgs)
{
    char_t *tdname = (char_t *)file;
    thread_t *t = NULL;
    u64_t retadr = 0, filelen = 0;
 
    get_file_rvadrandsz(tdname, &kmachbsp, &retadr, &filelen);
    if(NULL == retadr || 0 == filelen)
    {
        return SYSSTUSERR;
    }

    t = krlnew_thread(tdname, (void *)APPRUN_START_VITRUALADDR, USERTHREAD_FLG,
                      PRILG_USR, PRITY_MIN, DAFT_TDUSRSTKSZ, DAFT_TDKRLSTKSZ);
    t = krlthread_execvl(t, tdname);
    if (NULL == t)
    {
        return SYSSTUSERR; 
    }

    //t->td_id = flgs;
    return (hand_t)(t->td_id);
}

void krlsve_core_exit_thread()
{
  
    krlsched_exit();
    krlschedul();
    return;
}

hand_t krlsve_core_retn_threadhand(void *tname)
{
    return SYSSTUSERR;
}

sysstus_t krlsve_core_retn_threadstats(hand_t thand, uint_t scode, uint_t data, buf_t buf)
{
    return SYSSTUSERR;
}

sysstus_t krlsve_core_set_threadstats(hand_t thand, uint_t scode, uint_t data, buf_t buf)
{

    return SYSSTUSERR;
}
