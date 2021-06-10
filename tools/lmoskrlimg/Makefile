###################################################################
#		主控自动化编译配置文件 Makefile			  #
#				彭东  ＠ 2012.08.13.23.50	  #
###################################################################

MAKEFLAGS =-sR

MKDIR = mkdir
RMDIR = rmdir
CP = cp
CD = cd
DD = dd
RM = rm

MAKE = make
PREMENTMFLGS = -C $(BUILD_PATH) -f pretreatment.mk
KERNELMFLGS = -C $(BUILD_PATH) -f kernel.mk
IMGLINKFLGS = -C $(BUILD_PATH) -f krnllink.mk
CPFLAGES = 
DDFLAGS = 

BUILD_PATH = ./build
EXKNL_PATH = ./exckrnl
VM_PATH = ./vm
DSTPATH = ../exckrnl
RELEDSTPATH = ../release

.PHONY : print clean all

build: clean print all

all:
	$(MAKE) $(PREMENTMFLGS)
	$(MAKE) $(KERNELMFLGS)
	$(MAKE) $(IMGLINKFLGS)
	@echo '恭喜我，系统编译构建完成！ ^_^'

clean:
	$(CD) $(BUILD_PATH); $(RM) -f *.o *.bin *.i *.krnl *.s *.map *.lib *.btoj *.vdi *vmdk krnlobjs.mh
	@echo '清理全部已构建文件... ^_^'

print:
	@echo '*********正在开始编译构建系统*************'




