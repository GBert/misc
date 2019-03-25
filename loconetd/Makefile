# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=loconetd
PKG_RELEASE:=0.3.1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/loconetd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Loconet TCP/UDP Gateway
  AUTHOR:=Glenn Butcher
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/loconetd/description
  loconetd receives Loconet packets from a Locobuffer and UDP multicasts
  them to a Cat D group. It also receives raw packets via UDP unicast and
  retransmits them on the Loconet. Loconet is a proprietary network protocol
  for model railroad digital command and control (DCC) made by Digitrax,
  Inc. (http://www.digitrax.com).
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99"
endef

define Package/loconetd/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/loconetd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lococlient $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/loconetd.conf $(1)/etc
	$(INSTALL_BIN) ./files/loconetd.init $(1)/etc/init.d
endef

$(eval $(call BuildPackage,loconetd))
