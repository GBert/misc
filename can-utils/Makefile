#
# Copyright (C) 2018 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=can-utils
PKG_REV:=33a9249
PKG_REV:=2328c6c
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=2

PKG_SOURCE_MIRROR:=0 
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_URL:=https://github.com/linux-can/can-utils.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)/trunk/can-utils
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= CAN userspace utilities and tools
  URL:=http://gitorious.org/linux-can/can-utils
endef

MAKE_OPTS:= \
	PATH="$(TARGET_PATH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)/trunk/can-utils"

define Build/Compile
	$(MAKE)	-C $(PKG_BUILD_DIR)/trunk/can-utils CC=$(TARGET_CC) CFLAGS="$(TARGET_CFLAGS) -Iinclude -DSO_RXQ_OVFL=40 -DPF_CAN=29 -DAF_CAN=PF_CAN" $(TARGET_CONFIGURE_OPTS) $(MAKE_OPTS)
endef

# if you need more, add following below
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/asc2log $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/bcmserver $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/canbusload $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/candump $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cangen $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cangw $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/canlogserver $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/canplayer $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cansend $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cansniffer $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/isotpdump $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/isotprecv $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/isotpsend $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/isotpsniffer $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/isotptun $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/log2asc $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/log2long $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/slcan_attach $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/slcand $(1)/usr/bin
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/slcanpty $(1)/usr/bin


define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/canbusload $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/candump $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cangen $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cangw $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/cansend $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/log2long $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/can-utils/slcand $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
