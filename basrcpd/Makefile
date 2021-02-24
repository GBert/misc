# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=basrcpd
PKG_VERSION:=2102

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
ifdef CONFIG_LINUX_4_19
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/srcpd.conf_spi0 $(1)/etc/srcpd.conf
else
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/srcpd.conf $(1)/etc
endif
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/srcpd.regcount $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) files/srcpd.init $(1)/etc/init.d/srcpd
	$(INSTALL_BIN) files/presume-ack.init $(1)/etc/init.d/presume-ack
endef

$(eval $(call BuildPackage,basrcpd))
