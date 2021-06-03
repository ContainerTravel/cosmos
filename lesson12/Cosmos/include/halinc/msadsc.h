/**********************************************************
        物理内存空间数组文件msadsc.h
***********************************************************
                彭东
**********************************************************/
#ifndef _MSADSC_H
#define _MSADSC_H
void msadsc_t_init(msadsc_t* initp);
void disp_one_msadsc(msadsc_t* mp);
bool_t ret_msadsc_vadrandsz(machbstart_t* mbsp,msadsc_t** retmasvp,u64_t* retmasnr);
void write_one_msadsc(msadsc_t* msap,u64_t phyadr);
LKINIT u64_t init_msadsc_core(machbstart_t* mbsp,msadsc_t* msavstart,u64_t msanr);
LKINIT void init_msadsc();
void disp_phymsadsc();
u64_t search_segment_occupymsadsc(msadsc_t* msastart,u64_t msanr,u64_t ocpystat,u64_t ocpyend);
bool_t search_krloccupymsadsc_core(machbstart_t* mbsp);
void init_search_krloccupymm(machbstart_t* mbsp);
#endif