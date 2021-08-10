#ifndef BUILDFILE_H
#define BUILDFILE_H
#include "config.h"
#ifdef CFG_X86_PLATFORM

#define BUILD_HALY_OBJS init_entry.o hal_start.o halinit.o halglobal.o\
                        halplatform.o bdvideo.o halcpuctrl.o halprint.o\
                        halmm.o halintupt.o kernel.o i8259.o halgdtidt.o\
                        memmgrinit.o memdivmer.o memarea.o msadsc.o\
                        kmsob.o halmmu.o
#define BUILD_KRNL_OBJS krlinit.o krlvadrsmem.o krlglobal.o krlmm.o krlpagempol.o\
                        krlsem.o krlspinlock.o krlwaitlist.o krlsched.o krlthread.o\
                        krlcpuidle.o krldevice.o krlintupt.o krlobjnode.o krlservice.o\
                        krltime.o krlsveopen.o krlsveclose.o krlsveread.o krlsvewrite.o\
                        krlsvethread.o krlsvemm.o krlsvelseek.o krlsveioctrl.o krlstr.o
#define BUILD_MEMY_OBJS
#define BUILD_FSYS_OBJS
#define BUILD_DRIV_OBJS drvtick.o drvrfs.o drvuart.o
#define BUILD_LIBS_OBJS lapimm.o lapithread.o lapiopen.o lapiclose.o\
                        lapiread.o lapiwrite.o lapiioctrl.o lapilseek.o\
                        lapitime.o\
                        libmm.o libthread.o libopen.o libclose.o\
                        libread.o libwrite.o libioctrl.o liblseek.o\
                        libtime.o printf.o start.o
#define BUILD_APPS_OBJS oneuser.o helloworld.o

#define BUILD_LINK_OBJS BUILD_HALY_OBJS\
                        BUILD_KRNL_OBJS BUILD_MEMY_OBJS\
                        BUILD_FSYS_OBJS BUILD_DRIV_OBJS\

#define LINKR_IPUT_FILE BUILD_LINK_OBJS
#define LINKR_OPUT_FILE Cosmos.elf
#define KERNL_ELFF_FILE LINKR_OPUT_FILE
#define KERNL_BINF_FILE Cosmos.bin

#endif
                          
#endif // BUILDFILE_H
