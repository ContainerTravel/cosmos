/**********************************************************
        内核层中断处理头文件krlintupt.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KRLINTUPT_H
#define _KRLINTUPT_H
intserdsc_t* krladd_irqhandle(void* device,intflthandle_t handle,uint_t phyiline);
drvstus_t krlenable_intline(uint_t ifdnr);
drvstus_t krldisable_intline(uint_t ifdnr);

#endif // KRLINTUPT_H
