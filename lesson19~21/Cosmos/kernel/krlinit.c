/****************************************************************
        Cosmos kernel全局初始化文件krlinit.c
*****************************************************************
                彭东 
****************************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"
void init_krl()
{
    init_krlmm();
	// init_krldevice();
    // init_krldriver();
	// init_krlsched();
    // init_ktime();
    // init_thread();
    // init_task();
    // init_krlcpuidle();

    // hal_enable_irqfiq();
   
    die(0);
    return;
}
