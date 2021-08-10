/**********************************************************
        文件管理API文件lapiopen.c
***********************************************************
                彭东 
**********************************************************/
#include "libtypes.h"
#include "libheads.h"


hand_t api_open(void* file,uint_t flgs,uint_t stus)
{
    hand_t rethand;
    API_ENTRY_PARE3(INR_FS_OPEN,rethand,file,flgs,stus);
    return rethand;
}