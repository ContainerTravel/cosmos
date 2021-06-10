###################################################################
#		krnllink自动化编译配置文件 Makefile		  	  #
#				彭东  ＠ 2012.08.15.10.30	  #
###################################################################
MAKEFLAGS =-sR
KERNELCE_PATH	= ../arch/intelx86/kernel/
HEADFILE_PATH = ../arch/intelx86/include/
KRNLBOOT_PATH = ../arch/intelx86/boot/
CCBUILDPATH	= $(KRNLBOOT_PATH)
include krnlbuidcmd.mh
include krnlobjs.mh

.PHONY : all everything build_kernel
all: build_kernel 

build_kernel:everything
	
everything :  $(BUILD_MK_LIMG_EBIN)
 
$(BUILD_MK_LIMG_EBIN): $(BUILD_MK_LIMG_LINK)  
	$(CC) -o $@ -static $(BUILD_MK_LIMG_LINK)

