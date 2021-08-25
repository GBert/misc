# Copyright (C) 2018 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=maecan-server
PKG_RELEASE:=1.3

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/maecan-server
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=MaeCAN Server
  DEPENDS:= +can2udp +php7 +php7-cgi
endef

define Package/maecan-server/description
  MaeCAN Server by Maximilian Goldschmidt
endef

define Build/Compile
endef

define Package/maecan-server/install
	$(INSTALL_DIR) $(1)/www/MaeCAN-Server
	$(CP) -r ./node $(1)/www/MaeCAN-Server
	$(CP) -r ./html $(1)/www/MaeCAN-Server
	$(LN) /www/MaeCAN-Server/html $(1)/www/maecan
	$(INSTALL_DIR) $(1)/www/MaeCAN-Server/config
	$(LN) /www/MaeCAN-Server/config $(1)/www/maeconf
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_DATA) ./files/maecan-server $(1)/etc/uci-defaults/90_maecan_server
endef

$(eval $(call BuildPackage,maecan-server))
