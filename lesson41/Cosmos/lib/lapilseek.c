/**********************************************************
        文件管理API文件lapilseek.c
***********************************************************
                彭东
**********************************************************/
#include "libtypes.h"
#include "libheads.h"

sysstus_t api_lseek(hand_t fhand,uint_t lofset,uint_t flgs)
{
    sysstus_t rets;
    API_ENTRY_PARE3(INR_FS_LSEEK,rets,fhand,lofset,flgs);
    return rets;
}