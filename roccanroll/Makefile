#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=roccanroll
PKG_REV:=d892c56d00c6
PKG_REV:=67fc50410b5d
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_MIRROR:=0 
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_URL:=http://code.google.com/p/usb2rail/
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= RocCANroll - Maerklin railroad framework
  URL:=https://code.google.com/p/usb2rail/
endef

define Build/Compile
#	mkdir -p $(PKG_BUILD_DIR)/new_framework/include
	$(MAKE)	-C $(PKG_BUILD_DIR)/new_framework $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/new_framework/drehscheibe/drehscheibe $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/new_framework/client_cs2eth/mrcs2eth $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/new_framework/client_ms1/mrms1 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/new_framework/client_ms2/mrms2 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/new_framework/client_zentrale/mrzentrale $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
