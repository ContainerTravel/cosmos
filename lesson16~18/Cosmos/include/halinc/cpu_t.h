/**********************************************************
        CPU相关的宏定义文件cpu_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef _CPU_T_H
#define _CPU_T_H

#ifdef CFG_X86_PLATFORM

typedef struct s_INTSTKREGS
{
    reg_t rvsgs;    
    reg_t rvsfs;
    reg_t rvses;
    reg_t rvsds; 
    reg_t parmv7;//edi
    reg_t parmv6;//esi 
    reg_t parmv5;//ebp
    reg_t parmv4;//ebx
    reg_t parmv3;//edx 
    reg_t parmv2;//ecx  
    reg_t parmv1;//eax
    reg_t rvseip;    
    reg_t rvscs;
    reg_t rvseflags;
    reg_t rvsesp;
    reg_t rvsss;
}intstkregs_t;

#endif


#endif // CPU_T_H
