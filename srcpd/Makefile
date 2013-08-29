# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
# http://sourceforge.net/projects/srcpd/files/srcpd/2.1.2/srcpd-2.1.2.tar.gz/download

PKG_NAME:=srcpd
PKG_VERSION:=2.1.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)$(PKG_VERSION)-tar.gz
PKG_SOURCE_URL:=@SF/srcpd
PKG_MD5SUM:=bda8741db2c99b247a371daeee29d667
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

# Sart make install at build time

define Package/srcp
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=srcpd
  DEPENDS:=
  URL:=http://srcpd.sourceforge.net/srcpd
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/srcpd/description
  SRCP is an communication protocol designed to integrate all model railroad systems.
  Further key features are full multiuser capabilities and simplified user interface
  development. IANA assigned TCP port 4303 to it.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"
endef

define Package/srcpd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/srcpd $(1)/usr/bin/
endef


$(eval $(call BuildPackage,srcpd))
