# Copyright (C) 2020 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ms1relay
PKG_RELEASE:=1.06

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Maerklin MS1 Relay
  DEPENDS:=
endef

define Package/$(PKG_NAME)/description
  MS1 <-> MS2/CS2 Relay by Rainer Mueller
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ms1relay $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/ms1relay.init $(1)/etc/init.d/ms1relay
	$(INSTALL_DIR) $(1)/www/ms1relay/html
	$(INSTALL_BIN) ./files/ms1Display.css $(1)/www/ms1relay/html
	$(INSTALL_BIN) ./files/ms1Display.pl $(1)/www/ms1relay/html
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_DATA) ./files/ms1relay $(1)/etc/uci-defaults/90_ms1relay
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
