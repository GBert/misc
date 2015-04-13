# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=can2udp
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/can2udp
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=can2udp/can2lan gateway
  DEPENDS:= +zlib
endef

define Package/can2udp/description
  can2udp bridges CAN frames to UDP and vice versa
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99"
endef

define Package/can2udp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can2udp $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can2lan $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gb2-update $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init $(1)/etc/init.d/can2lan
endef

define Package/can2udp/postinst
        ln -sf /usr/bin/gb2-update $${IPKG_INSTROOT}/usr/bin/ms2-update
endef

$(eval $(call BuildPackage,can2udp))
