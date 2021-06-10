#include "imgmctrl.h"

void init_mem()
{
    return;
}

void exit_mem()
{
    return;
}

void* img_mem(long msize,void* mfradr,uint_t mflg)
{
    if(mflg==MFLG_ALLOC)
    {
        return malloc(msize);
    }
    if(mflg==MFLG_FREE)
    {
        free(mfradr);
        return (void*)1;
    }
    return NULL;
}

void limg_memcpy(void*dest,const void *src,size_t n)
{
    void* buf=memcpy(dest,src,n);
    if(buf==dest)
    {
        return;
    }
    return;
}

void limg_memclr(void* p,int c,size_t n)
{
    void *buf=memset(p,c,n);
    if(buf==NULL)
    {
        limg_error("memclr");
    }
    return;
}

