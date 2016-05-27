#Makefile for kernel module for kernel 2.6
ifneq ($(KERNELRELEASE),)
    obj-m	:= cygnal_c2.o    
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif
