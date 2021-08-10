/**********************************************************
        内核服务文件krlservice.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"
sysstus_t krlservice(uint_t inr, void* sframe)
{
    if(INR_MAX <= inr)
    {
        return SYSSTUSERR;
    }
    if(NULL == osservicetab[inr])
    {
        return SYSSTUSERR;
    }
    return osservicetab[inr](inr, (stkparame_t*)sframe);
}