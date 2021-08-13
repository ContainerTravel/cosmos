/**********************************************************
        线程管理API文件lapithread.c
***********************************************************
                彭东
**********************************************************/
#include "libtypes.h"
#include "libheads.h"


hand_t api_exel_thread(void* file,uint_t flgs)
{
    hand_t rethand;
    API_ENTRY_PARE2(INR_TD_EXEL,rethand,file,flgs);
    return rethand;
}

void api_exit_thread()
{
    uint_t rs,pv1=0;
    API_ENTRY_PARE1(INR_TD_EXIT,rs,pv1);
    return;
}


hand_t api_retn_threadhand(void* tname)
{
    hand_t rethand;
    API_ENTRY_PARE1(INR_TD_HAND,rethand,tname);
    return rethand;
}

sysstus_t api_retn_threadstats(hand_t thand,uint_t scode,uint_t data,buf_t buf)
{
    sysstus_t rets;
    API_ENTRY_PARE4(INR_TD_RSTATS,rets,thand,scode,data,buf);
    return rets;
}

sysstus_t api_set_threadstats(hand_t thand,uint_t scode,uint_t data,buf_t buf)
{
    sysstus_t rets;
    API_ENTRY_PARE4(INR_TD_SSTATS,rets,thand,scode,data,buf);
    return rets;
}
