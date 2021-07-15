
ifneq ($(KERNELRELEASE),)
obj-m :=miscdrv.o
else
KDIR :=/lib/modules/$(shell uname -r)/build
all: clean buildapp
	make -C $(KDIR) M=$(PWD) modules
buildapp:
	gcc ./app.c -o app
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order app
endif
