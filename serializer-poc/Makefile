#
# Copyright (C) 2013 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=serializer
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/serializer
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= Ralink RT305x/RT5350 I2S Bit Stream Serializer
  URL:=http://lnxpps.de/openwrt/serializer
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/serializer/description
  Proof of Concept - Bit Stream Serializer to generater bit streams defined thru clockrate and bits
  via I2S
  Ralink low cost WLAN SoCs offer an I2S interface which could be used to generate bitstream. Because
  of the wide range of frequency (20MHz down to 20Khz) and the 32 byte FIFO the I2S offers an
  interesting field for experiments ...
  The code uses Zilvinas Valinskas mmio tool as base to manipulate the I2S registers. 
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

define Package/serializer/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/serializer $(1)/usr/bin/
endef

$(eval $(call BuildPackage,serializer))
