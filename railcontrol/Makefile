# Copyright (C) 2020 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# find ./ -regextype egrep -regex '.*/*.c|.*/*.cpp|.*/*.h|.*/*.css|.*/*.html|.*/*.png|.*/*.svg|.*/*.js|.*/*.ico' | \
#               sort | xargs tar -cvf railcontrol_files.tar
#


include $(TOPDIR)/rules.mk

PKG_NAME:=railcontrol
PKG_VERSION:=20210306

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

# Sart make install at build time

define Package/railcontrol
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=railcontrol
  DEPENDS:=+libpthread +libstdcpp +libsqlite3
  URL:=https://www.railcontrol.org
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/srcpd/description
  Railcontrol is a software to control model railroad by
  Dominik (Teddy) Mahrer
endef


define Build/Prepare
        $(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CPPFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -I. -I.. -fPIC -Dlinux -D_REENTRANT -lpthread -ldl" 
endef

define Package/railcontrol/install
	$(INSTALL_DIR) $(1)/opt/railcontrol
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/railcontrol $(1)/opt/railcontrol
	$(INSTALL_BIN) ./files/railcontrol.sqlite $(1)/opt/railcontrol
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/railcontrol.conf.dist $(1)/opt/railcontrol/railcontrol.conf
	$(INSTALL_DIR) $(1)/opt/railcontrol/html
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/html/* $(1)/opt/railcontrol/html/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/railcontrol.init $(1)/etc/init.d/railcontrol
endef


$(eval $(call BuildPackage,railcontrol))
