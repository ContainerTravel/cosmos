/**********************************************************
        COSMOS内存管理API文件lapimm.c
***********************************************************
                彭东
**********************************************************/
#include "libtypes.h"
#include "libheads.h"

void* api_mallocblk(size_t blksz)
{
    void* retadr;
    API_ENTRY_PARE1(INR_MM_ALLOC,retadr,blksz);
    return retadr;
}

sysstus_t api_mfreeblk(void* fradr,size_t blksz)
{
    sysstus_t retstus;
    API_ENTRY_PARE2(INR_MM_FREE,retstus,fradr,blksz);
    return retstus;
}