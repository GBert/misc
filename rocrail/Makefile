# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rocrail
PKG_REV:=5865
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://bazaar.launchpad.net/~rocrail-project/rocrail/Rocrail
PKG_SOURCE_PROTO:=bzr
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/rocrail
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Rocrail
  DEPENDS:=
endef

define Package/rocrail/description
  Rocrail - Innovative Model Railroad Control System
endef

define Build/Compile
        cd $(PKG_BUILD_DIR)/rocs
	make fromtar PLATFORM=LINUX all
	cp bin/ogen bin/ogen_x86
	cp bin/wgen bin/wgen_x86
	cp bin/xml2cstr bin/xml2cstr_x86
	cp bin/png2wx bin/png2wx_x86
	make clean
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)" 
endef

define Package/rocrail/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rocrail $(1)/opt/rocrail
endef

$(eval $(call BuildPackage,rocrail))
