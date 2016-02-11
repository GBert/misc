#
# Copyright (C) 2015 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=susit
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/susit
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= SUSI Tool
  DEPENDS:=+kmod-susi-gpio
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/susit/description
  SUSI Tool via 2 GPIOs
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	make -C $(PKG_BUILD_DIR)		\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"	\
		LIBS="$(TARGET_LDFLAGS)"
endef

define Package/susit/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/susit $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./files/susi.sh $(1)/root/
endef

$(eval $(call BuildPackage,susit))
