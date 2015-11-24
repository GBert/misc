# Copyright (C) 2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ftdi2s88
PKG_RELEASE:=1.00

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/ftdi2s88
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=USB to S88 Gateway
  DEPENDS:=+libftdi1
endef

define Package/ftdi2s88/description
  USB to S88 sgGatway using FTDI USB to serial adapter
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

TARGET_CFLAGS+= -I$(STAGING_DIR)/usr/include/libftdi1 \


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99"
endef

define Package/ftdi2s88/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ftdi2s88 $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ftdi2s88))
