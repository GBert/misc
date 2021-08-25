#
# Copyright (C) 2021 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=xpndev
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Other modules
  TITLE:=XpressNet Device Driver for Allwinner SoC
  DEPENDS:=@GPIO_SUPPORT
  FILES:=$(PKG_BUILD_DIR)/xpndev.ko
  KCONFIG:=
endef

define KernelPackage/$(PKG_NAME)/description
 Kernel module that implements a XpressNet device (aka RS485 9bit/multidrop 62500 baud)
endef

EXTRA_KCONFIG:= \
	CONFIG_XPNDEV=m

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	$(KERNEL_MAKE_FLAGS) \
	M="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
