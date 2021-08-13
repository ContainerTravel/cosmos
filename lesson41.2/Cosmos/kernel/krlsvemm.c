/**********************************************************
        内核服务文件krlsvemm.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

sysstus_t krlsvetabl_mallocblk(uint_t inr, stkparame_t *stkparv)
{

    if (inr != INR_MM_ALLOC)
    {
        return SYSSTUSERR;
    }
    return (sysstus_t)krlsve_mallocblk((size_t)stkparv->parmv1);
}

sysstus_t krlsvetabl_mfreeblk(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_MM_FREE)
    {
        return SYSSTUSERR;
    }
    return krlsve_mfreeblk((void *)stkparv->parmv1, (size_t)stkparv->parmv2);
}

void *krlsve_mallocblk(size_t blksz)
{
   
    return krlsve_core_mallocblk(blksz);
}

sysstus_t krlsve_mfreeblk(void *fradr, size_t blksz)
{
    if (fradr == NULL)
    {
        return SYSSTUSERR;
    }
    return krlsve_core_mfreeblk(fradr, blksz);
}

void *krlsve_core_mallocblk(size_t blksz)
{
    adr_t retvadr = vma_new_vadrs(krl_curr_mmadrsdsc(), NULL, blksz, 0, KMV_HEAP_TYPE);
    // kprint("krlsve_core_mallocblk:%x :%x\n", retvadr, blksz);
    return (void *)retvadr;
}

sysstus_t krlsve_core_mfreeblk(void *fradr, size_t blksz)
{
    if (vma_del_vadrs(krl_curr_mmadrsdsc(), (adr_t)fradr, blksz) == FALSE)
    {
        return SYSSTUSERR;
    }
    // kprint("krlsve_core_mfreeblk:%x :%x\n", (uint_t)fradr, blksz);

    return SYSSTUSOK;
}