/**********************************************************
        cp15协处理器芯片操作头文件halmmu.h
***********************************************************
                彭东
**********************************************************/
#ifndef _HALMMU_H
#define _HALMMU_H
void hal_disable_cache();
u32_t cp15_read_c5();
u32_t cp15_read_c6();
u32_t hal_read_cp15regs(uint_t regnr);
#endif // HALMMU_H
