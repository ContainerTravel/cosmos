/**********************************************************
        物理内存管理器初始化文件memmgrinit.h
***********************************************************
                彭东 
**********************************************************/
#ifndef _MEMMGRINIT_H
#define _MEMMGRINIT_H
LKINIT void init_memmgr();
void disp_memmgrob();
void init_memmgrob();
void memmgrob_t_init(memmgrob_t* initp);
bool_t copy_pages_data(machbstart_t* mbsp);
bool_t copy_fvm_data(machbstart_t* mbsp,dftgraph_t* dgp);
void init_copy_pagesfvm();
#endif