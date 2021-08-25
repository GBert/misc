#
# Copyright (C) 2015 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pwm-pulse
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/pwm-pulse
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= Allwinner PWM Pulse Test
  DEPENDS:=
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/pwm-pulse/description
  Allwinner PWM Pulse Test
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

ifneq (,$(findstring sun7i-a20,$(CONFIG_TARGET_PROFILE)))
        TARGET_CPPFLAGS+= -DSUNXI_A20
endif

define Build/Compile
	make -C $(PKG_BUILD_DIR)		\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"	\
		LIBS="$(TARGET_LDFLAGS)"
endef

define Package/pwm-pulse/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pwm-pulse $(1)/usr/bin/
endef

$(eval $(call BuildPackage,pwm-pulse))
