###################################################################
#		APP自动化编译配置文件 Makefile                    #
#				彭东  	  #
###################################################################

MAKEFLAGS =
KERNELCE_PATH	= ../apps/
HEADFILE_PATH = -I ../include -I ../include/bastypeinc -I ../include/halinc -I ../include/knlinc -I ../include/libinc -I ../include/drvinc
CCBUILDPATH	= $(KERNELCE_PATH)
include krnlbuidcmd.mk
include krnlobjs.mk

APPS := $(addsuffix .app, $(basename $(BUILD_MK_APPS_OBJS)))
.PHONY : all everything  build_kernel linkapp
all: build_kernel linkapp

build_kernel:everything

everything :$(BUILD_MK_APPS_OBJS)

linkapp: $(APPS)

%.app:
[make -f appslink.mk OBJ=$@
include krnlbuidrule.mk
