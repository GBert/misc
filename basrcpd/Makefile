# Copyright (C) 2018 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
# http://sourceforge.net/projects/srcpd/files/srcpd/2.1.2/srcpd-2.1.2.tar.gz/download

PKG_NAME:=basrcpd
PKG_VERSION:=1902

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

# Sart make install at build time

define Package/basrcpd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=basrcpd
  DEPENDS:=+libxml2 +libpthread
  URL:=http://srcpd.sourceforge.net/srcpd
  MAINTAINER:=Rainer Mueller
endef

define Package/srcpd/description
  SRCP is an communication protocol designed to integrate all model railroad systems.
  Further key features are full multiuser capabilities and simplified user interface
  development. IANA assigned TCP port 4303 to it.
  Special SRCPD version from Rainer Mueller
endef

define Build/Configure
  $(call Build/Configure/Default,--disable-ddls88)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -Dlinux -D_REENTRANT -I$(STAGING_DIR)/usr/include/libxml2 -I./ -lpthread"
endef

define Package/basrcpd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/srcpd $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/srcpd.conf $(1)/etc
endef


$(eval $(call BuildPackage,basrcpd))
