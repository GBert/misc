# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
# http://sourceforge.net/projects/srcpd/files/srcpd/2.1.2/srcpd-2.1.2.tar.gz/download

PKG_NAME:=srcpd
PKG_VERSION:=2.1.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/srcpd/srcpd/2.1.3
PKG_MD5SUM:=2f6c173e479ece56cdd5b97f8d0a257c
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

# Sart make install at build time

define Package/srcpd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=srcpd
  DEPENDS:=+libxml2 +libpthread
  URL:=http://srcpd.sourceforge.net/srcpd
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/srcpd/description
  SRCP is an communication protocol designed to integrate all model railroad systems.
  Further key features are full multiuser capabilities and simplified user interface
  development. IANA assigned TCP port 4303 to it.
endef

define Build/Configure
  $(call Build/Configure/Default,--disable-ddls88)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -lpthread"
endef

define Package/srcpd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/srcpd $(1)/usr/bin/
endef


$(eval $(call BuildPackage,srcpd))
