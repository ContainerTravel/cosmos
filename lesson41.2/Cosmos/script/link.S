###################################################################
#		krnllink自动化编译配置文件 Makefile               #
#				彭东 #
###################################################################
MAKEFLAGS =

include krnlbuidcmd.mk
include krnlobjs.mk

.PHONY : all everything build_kernel
all: build_kernel

build_kernel:everything

everything :   
[$(LD) $(LDFLAGS) -o $(KERNL_MK_ELFF_FILE) $(BUILD_MK_LINK_OBJS)
[$(OBJCOPY) $(OJCYFLAGS) $(KERNL_MK_ELFF_FILE) $(KERNL_MK_BINF_FILE)
