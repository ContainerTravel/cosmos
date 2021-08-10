/**********************************************************
        文件管理API文件lapiclose.c
***********************************************************
                彭东
**********************************************************/
#include "libtypes.h"
#include "libheads.h"

sysstus_t api_close(hand_t fhand)
{

    sysstus_t rets;
    API_ENTRY_PARE1(INR_FS_CLOSE,rets,fhand);
    return rets;
}