MAKEFLAGS =


include krnlbuidcmd.mk
include krnlobjs.mk

OBJ :=
ELF_FILE := $(basename $(OBJ)).elf
LINKOBJS := $(BUILD_MK_LIBS_OBJS) $(basename $(OBJ)).o
APP_FILE := $(basename $(OBJ)).app
APPLDFLAGS	= -s -static -T applds.lds -n -Map $(APP_FILE).map
.PHONY : all everything build_kernel
all: build_kernel

build_kernel:everything

everything : $(APP_FILE)

%.elf:
[$(LD) $(APPLDFLAGS) -o $@ $(LINKOBJS)

%.app:%.elf
[$(OBJCOPY) $(OJCYFLAGS) $< $@

include krnlbuidrule.mk
