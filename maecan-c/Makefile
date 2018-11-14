# Copyright (C) 2018 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=maecan-c
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/maecan-c
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=MaeCAN C Server
  DEPENDS:= +libwebsockets +libpthread
endef

define Package/maecan-server/description
  MaeCAN C-Server by Maximilian Goldschmidt
endef

define Build/Prepare
        $(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99"
endef

define Package/maecan-c/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/maecanserver $(1)/usr/bin/
#	$(CP) -r ./html $(1)/www/MaeCAN-Server
#	$(LN) /www/MaeCAN-Server/html $(1)/www/maecan
#	$(INSTALL_DIR) $(1)/www/MaeCAN-Server/config
#	$(LN) /www/MaeCAN-Server/config $(1)/www/maeconf
#	$(INSTALL_DIR) $(1)/etc/uci-defaults
#	$(INSTALL_DATA) ./files/maecan-server $(1)/etc/uci-defaults/90_maecan_server
endef

$(eval $(call BuildPackage,maecan-c))
