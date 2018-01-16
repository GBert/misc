# Copyright (C) 2018 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=maecan-server
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/maecan-server
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=MaeCAN Server
  DEPENDS:= +can2udp +php7 +node
endef

define Package/maecan-server/description
  MaeCAN Server
endef

define Build/Compile
endef

define Package/maecan-server/install
	$(INSTALL_DIR) $(1)/www/MaeCAN-Server
	$(CP) -r ./node $(1)/www/MaeCAN-Server
	$(CP) -r ./html $(1)/www/MaeCAN-Server
endef

define Package/maecan-server/postinst
	sed -i 's,doc_root.*,doc_root = "",g' /etc/php.ini
	sed -i 's,;short_open_tag = Off,short_open_tag = On,g' /etc/php.ini
	uci set uhttpd.llmp=uhttpd
	uci set uhttpd.llmp.listen_http=81
	uci set uhttpd.llmp.home=/www/MaeCAN-Server/html
	uci add_list uhttpd.llmp.interpreter=".php=/usr/bin/php-cgi"
	uci set uhttpd.llmp.index_page="index.html index.htm default.html default.htm index.php"
	uci commit uhttpd
endef

$(eval $(call BuildPackage,maecan-server))
