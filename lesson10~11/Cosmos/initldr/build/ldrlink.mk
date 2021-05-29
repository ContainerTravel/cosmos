include krnlbuidcmd.mh
include ldrobjs.mh
.PHONY : all everything build_kernel img
all: build_kernel 
#INITLDR
build_kernel:everything build_bin img
everything : $(INITLDRIMH_ELF) $(INITLDRKRL_ELF) $(INITLDRSVE_ELF)
build_bin:$(INITLDRIMH) $(INITLDRKRL) $(INITLDRSVE)

$(INITLDRIMH_ELF): $(INITLDRIMH_LINK)
	$(LD) $(LDRIMHLDFLAGS) -o $@ $(INITLDRIMH_LINK)
$(INITLDRKRL_ELF): $(INITLDRKRL_LINK)
	$(LD) $(LDRKRLLDFLAGS) -o $@ $(INITLDRKRL_LINK)
$(INITLDRSVE_ELF): $(INITLDRSVE_LINK)
	$(LD) $(LDRSVELDFLAGS) -o $@ $(INITLDRSVE_LINK)
$(INITLDRIMH):$(INITLDRIMH_ELF)
	$(OBJCOPY) $(OJCYFLAGS) $< $@
	@echo 'OBJCOPY -[M] 正在构建...' $@  
$(INITLDRKRL):$(INITLDRKRL_ELF)
	$(OBJCOPY) $(OJCYFLAGS) $< $@
	@echo 'OBJCOPY -[M] 正在构建...' $@ 
$(INITLDRSVE):$(INITLDRSVE_ELF)
	$(OBJCOPY) $(OJCYFLAGS) $< $@
	@echo 'OBJCOPY -[M] 正在构建...' $@ 

img:
	./lmoskrlimg -m k -lhf $(INITLDRIMH) -o Cosmos.eki -f $(INITLDRKRL) $(INITLDRSVE)