/****************************************************************
        Cosmos HAL全局初始化文件halinit.c
*****************************************************************
                彭东
****************************************************************/

#include "cosmostypes.h"
#include "cosmosmctrl.h"


void init_hal()
{

    init_halplaltform();
    move_img2maxpadr(&kmachbsp);
    init_halmm();
    init_halintupt();
    return;
}
