###################################################################
#		kernel自动化编译配置文件 Makefile			  #
#				彭东  ＠ 2012.08.17.10.50	  #
###################################################################

MAKEFLAGS = -s
KERNELCE_PATH	= ../core/
HEADFILE_PATH = ../include/

CCBUILDPATH	= $(KERNELCE_PATH)
include krnlbuidcmd.mh
include krnlobjs.mh

.PHONY : all everything  build_kernel
all: build_kernel 

build_kernel:everything
	
everything : $(BUILD_MK_CORE_OBJS) 

	
include krnlbuidrule.mh
