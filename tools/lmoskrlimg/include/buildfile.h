/**********************************************************
        内核编译文件头文件buildfile.h
***********************************************************
                彭东 ＠ 2014.02.07.16.00
**********************************************************/
#ifndef _BUILDFILE_H
#define _BUILDFILE_H
#define BUILD_CORE_OBJS lmoskrlimg.o imgcore.o imgmgrhead.o param.o file.o\
    imgundo.o memdisk.o mem.o limgerror.o
#define BUILD_LIMG_LINK BUILD_CORE_OBJS


#define BUILD_LIMG_EBIN lmoskrlimg



#endif // _BUILDFILE_H
