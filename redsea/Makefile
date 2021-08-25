#
# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=redsea
PKG_REV:=57ce01f 
PKG_RELEASE:=0.17.1
PKG_VERSION:=0.17.1

PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/windytan/redsea.git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= RDS decoder
  DEPS:=+liquid-dsp
  URL:=http://www.windytan.com/2016/10/redsea-07-lightweight-rds-decoder.html
endef

define Build/Configure
	( cd $(PKG_BUILD_DIR); ./autogen.sh)
	$(call Build/Configure/Default)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/trunk/redsea $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
