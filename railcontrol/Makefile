# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=railcontrol
PKG_VERSION:=20200330

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

# Sart make install at build time

define Package/railcontrol
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=railcontrol
  DEPENDS:=
  URL:=https://www.railcontrol.org
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/srcpd/description
  Railcontrol is a software to control model railroad by
  Dominik (Teddy) Mahrer
endef

define Build/Configure
  $(call Build/Configure/Default,--disable-ddls88)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -Dlinux -D_REENTRANT -I$(STAGING_DIR)/usr/include/libxml2 -I./ -lpthread"
endef

define Package/railcontrol/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/railcontrol $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/railcontrol.conf.dist $(1)/etc/railcontrol.conf
endef


$(eval $(call BuildPackage,railcontrol))
