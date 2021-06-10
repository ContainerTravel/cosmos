#ifndef _MEM_H
#define _MEM_H
#define MFLG_ALLOC 1
#define MFLG_FREE 2
void init_mem();
void exit_mem();
void* img_mem(long msize,void* mfradr,uint_t mflg);
void limg_memcpy(void*dest,const void *src,size_t n);
void limg_memclr(void* p,int c,size_t n);
#endif
