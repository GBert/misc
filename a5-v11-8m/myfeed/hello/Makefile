#
# Copyright (C) 2017 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=hello
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/hello
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= Famous Helle World
  DEPENDS:=
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/hello/description
  The famous Hello World
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

define Package/hello/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hello $(1)/usr/bin/
endef

$(eval $(call BuildPackage,hello))
