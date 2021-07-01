/****************************************************************
        LMOSEM HAL全局数据结构文件halglobal.c
*****************************************************************
                彭东
****************************************************************/
#define HALGOBAL_HEAD
#include "cosmostypes.h"
#include "cosmosmctrl.h"

#ifdef CFG_X86_PLATFORM
HAL_DEFGLOB_VARIABLE(descriptor_t,x64_gdt)[CPUCORE_MAX][GDTMAX];
HAL_DEFGLOB_VARIABLE(gate_t,x64_idt)[IDTMAX];
HAL_DEFGLOB_VARIABLE(x64tss_t,x64tss)[CPUCORE_MAX]; 
HAL_DEFGLOB_VARIABLE(igdtr_t,x64_igdt_reg)[CPUCORE_MAX];
HAL_DEFGLOB_VARIABLE(iidtr_t,x64_iidt_reg);
HAL_DEFGLOB_VARIABLE(machbstart_t, kmachbsp);
HAL_DEFGLOB_VARIABLE(dftgraph_t, kdftgh);
HAL_DEFGLOB_VARIABLE(memmgrob_t, memmgrob);
HAL_DEFGLOB_VARIABLE(intfltdsc_t, machintflt)[IDTMAX];
#endif

