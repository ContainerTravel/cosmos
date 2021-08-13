/**********************************************************
        文件管理API文件lapiread.c
***********************************************************
                彭东 
**********************************************************/
#include "libtypes.h"
#include "libheads.h"


sysstus_t api_read(hand_t fhand,buf_t buf,size_t len,uint_t flgs)
{
    sysstus_t rets;
    API_ENTRY_PARE4(INR_FS_READ,rets,fhand,buf,len,flgs);
    return rets;
}