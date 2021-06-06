/****************************************************************
        HAL内存初始化头文件halmm.h
*****************************************************************
                彭东
****************************************************************/
#ifndef _HALMM_H
#define _HALMM_H

#ifdef CFG_X86_PLATFORM
PUBLIC void phymmarge_t_init(phymmarge_t* initp);
PUBLIC void ret_phymmarge_adrandsz(machbstart_t* mbsp,phymmarge_t** retpmrvadr,u64_t* retpmrsz);
PUBLIC bool_t init_one_pmrge(e820map_t* e8p,phymmarge_t* pmargep);
void phymmarge_swap(phymmarge_t* s,phymmarge_t* d);
void phymmarge_sort(phymmarge_t* argp,u64_t nr);
PUBLIC u64_t initpmrge_core(e820map_t* e8sp,u64_t e8nr,phymmarge_t* pmargesp);

PUBLIC void init_phymmarge();

void init_halmm();
#endif
#endif // _HALINIT_H
