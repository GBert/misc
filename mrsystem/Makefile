#
# Copyright (C) 2020 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mrsystem
PKG_VERSION:=2.7

PKG_MD5SUM:=461d96195d67988eb38226d428470ec0
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= Model Railroad System
  URL:=http://mbernstein.de/modellbahn/bbb.htm
  DEPENDS:= +zlib
  TARGET_CFLAGS += -ggdb3
endef

define Package/mrsystem/description
  MRSystem by Michael Bernstein http://mbernstein.de/modellbahn/can/index.htm
endef

define Build/Prepare
        $(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)/basic_libs/include
	mkdir -p $(PKG_BUILD_DIR)/basic_libs/lib
	mkdir -p $(PKG_BUILD_DIR)/include
	mkdir -p $(PKG_BUILD_DIR)/lib
	$(MAKE)	-C $(PKG_BUILD_DIR)/basic_libs $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"
	$(MAKE)	-C $(PKG_BUILD_DIR)/../mrsystem $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -I$(PKG_BUILD_DIR)/basic_libs/include -L$(PKG_BUILD_DIR)/basic_libs/lib"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/drehscheibe/drehscheibe $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_cs2eth/mrcs2eth $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_cs2sl/mrcs2sl $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_inject/mrinject $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_log/mrlog $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_logms2/mrlogms2 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_ms2/mrms2 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_zentrale/release/mrzentrale $(1)/usr/bin
	$(INSTALL_DIR) $(1)/root
	$(CP) ./files/mrsystem.init $(1)/root/
	$(INSTALL_DIR) $(1)/etc
	$(CP) ./files/mrsystem $(1)/etc/
	$(INSTALL_DIR) $(1)/www/
	$(CP) ./files/www/* $(1)/www/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
