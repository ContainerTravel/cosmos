#include "imgmctrl.h"
int main(int argc, char *argv[])
{
    init_lmoskrlimg();
    limg_param(argc,argv);
    limg_core();
    exit_lmoskrlimg();
	return 0;
}

void init_lmoskrlimg()
{
    init_error();
    init_param();
    init_imgundo();
    init_file();
    init_mem();
    init_imgcore();
    init_imgmgrhead();

    return;
}

void new_testfile()
{
    uint_t retsz=limg_createfile_setval("test1.t",0x1000,1,0);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    retsz=limg_createfile_setval("test2.t",0x800,1,0xaa);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    retsz=limg_createfile_setval("test3.t",0x1000,1,0xbb);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    retsz=limg_createfile_setval("test4.t",0x400,1,0xcc);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    retsz=limg_createfile_setval("test5.t",0x200,1,0xdd);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    retsz=limg_createfile_setval("test6.t",0x1000,1,0xee);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    retsz=limg_createfile_setval("test7.t",0x1000,1,0xff);
    if(retsz==0)
    {
        limg_error("cn file");
    }
    return;
}
void exit_lmoskrlimg()
{
    exit_imgmgrhead();
    exit_imgcore();
    exit_mem();
    exit_file();
    exit_imgundo();
    exit_param();
    exit_error();
    return;
}

