#
# Copyright (C) 2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=s88-maerklin
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/s88-maerklin
  SUBMENU:=Other modules
  TITLE:=Railroad Maerklin S88 Kernel Modul
  DEPENDS:=@GPIO_SUPPORT
  FILES:=$(PKG_BUILD_DIR)/s88-main.ko
  MAINTAINER:=Gerhard Bertelsmann
  KCONFIG:=
endef

define KernelPackage/s88-maerklin/description
 Railroad Maerklin S88 GPIO Kernel module 
endef

EXTRA_KCONFIG:= \
    CONFIG_S88_MAERKLIN=m

EXTRA_CFLAGS:= \
    $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
    $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
    ARCH="$(LINUX_KARCH)" \
    CROSS_COMPILE="$(TARGET_CROSS)" \
    SUBDIRS="$(PKG_BUILD_DIR)" \
    EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
    $(EXTRA_KCONFIG)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) modules
endef

# define Package/postinst
# #!/bin/sh
# mknod /dev/s88-maerklin c 180 0
# endef

$(eval $(call KernelPackage,s88-maerklin))
