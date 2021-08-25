#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=rs485bpi
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Other modules
  TITLE:=BPI RS485
  DEPENDS:=@GPIO_SUPPORT
  FILES:=$(PKG_BUILD_DIR)/rs485bpi.ko
  MAINTAINER:=Gerhard Bertelsmann
  KCONFIG:=
endef

define KernelPackage/rs485bpi/description
  BananaPi Multidrop capable RS485 interface
endef

EXTRA_KCONFIG:= \
	CONFIG_RS485_BPI=m

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	$(KERNEL_MAKE_FLAGS) \
	M="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
