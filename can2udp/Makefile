# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=can2udp
PKG_RELEASE:=0.1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/can2udp
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=can2udp gateway
  DEPENDS:=
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
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)" 
endef

define Package/can2udp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can2udp $(1)/usr/bin/
endef


$(eval $(call BuildPackage,can2udp))
