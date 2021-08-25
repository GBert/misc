#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=susi-gpio
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/susi-gpio
  SUBMENU:=Other modules
  TITLE:=susi GPIO kernel module
  DEPENDS:=@GPIO_SUPPORT
  FILES:=$(PKG_BUILD_DIR)/susi-gpio.ko
  KCONFIG:=
endef

define KernelPackage/susi-gpio/description
 Kernel module for susi
endef

EXTRA_KCONFIG:= \
    CONFIG_SUSI_GPIO=m \

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

$(eval $(call KernelPackage,susi-gpio))
