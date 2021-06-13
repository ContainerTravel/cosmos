/*###################################################################
#		hal自动化编译配置文件 Makefile                    #
#				彭东 	  #
###################################################################
*/
#include "config.h"
MAKEFLAGS = -s
#ifdef CFG_X86_PLATFORM
KERNELCE_PATH	= ../hal/x86/
HEADFILE_PATH = -I ../include -I ../include/bastypeinc -I ../include/halinc -I ../include/knlinc -I ../include/libinc -I ../include/drvinc
#endif
#ifdef CFG_S3C2440A_PLATFORM
KERNELCE_PATH	= ../hal/s3c2440/
HEADFILE_PATH = -I ../include -I ../include/bastypeinc -I ../include/halinc -I ../include/knlinc -I ../include/libinc -I ../include/drvinc
#endif
#ifdef CFG_STM32F0XX_PLATFORM
KERNELCE_PATH	= ../hal/stm32f0xx/
HEADFILE_PATH = -I ../include -I ../include/bastypeinc -I ../include/halinc -I ../include/knlinc -I ../include/libinc -I ../include/drvinc
#endif

CCBUILDPATH	= $(KERNELCE_PATH)
include krnlbuidcmd.mk
include krnlobjs.mk

.PHONY : all everything  build_kernel
all: build_kernel

build_kernel:everything

everything :$(BUILD_MK_HALY_OBJS)
include krnlbuidrule.mk
