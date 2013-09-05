# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rocrail
PKG_REV:=5874
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
  DEPENDS:=+lpthread
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/rocrail/description
  Rocrail - Innovative Model Railroad Control System
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/rocs fromtar PLATFORM=LINUX all
	cp $(PKG_BUILD_DIR)/rocs/bin/ogen $(PKG_BUILD_DIR)/rocs/bin/ogen_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/wgen $(PKG_BUILD_DIR)/rocs/bin/wgen_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/xml2cstr $(PKG_BUILD_DIR)/rocs/bin/xml2cstr_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/png2wx $(PKG_BUILD_DIR)/rocs/bin/png2wx_x86
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) GENSUFFIX=_x86 DEBUG= CC_EXTRA_FLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -fPIC" TOOLPREFIX=$(TARGET_CROSS) PLATFORM=LINUX server
endef

define Package/rocrail/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rocrail $(1)/opt/rocrail
endef

$(eval $(call BuildPackage,rocrail))
