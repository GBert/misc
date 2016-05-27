#Makefile for kernel module for kernel 2.6
ifneq ($(KERNELRELEASE),)
    obj-m	:= c2_gpio.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif
