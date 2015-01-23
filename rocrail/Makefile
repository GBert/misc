# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rocrail
PKG_REV:=2cf33d2119
PKG_REV:=94c0c29070
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://github.com/rocrail/Rocrail
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/rocrail
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Rocrail
  MENU:=1
  URL:=http://rocrail.net
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/rocrail/description
  Rocrail - Innovative Model Railroad Control System
endef

define Package/rocrail-server
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Rocrail server
  DEPENDS:=+libpthread +libusb-1.0 rocrail
endef

define Package/rocrail-server/description
 This package contains the Rocrail Server
endef

define Package/rocnetnode
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=rocnetnode
  DEPENDS:=+libpthread rocrail
endef

define Package/rocrail-node/description
 This package contains the rocnetnode
endef

define Package/rocrail-cs-mcs2
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station Maerklin CS2 
  DEPENDS:=rocrail-server
endef

define Package/rocrail-cs-mcs2/description
 Command Station Maerklin CS2
endef

define Package/rocrail-cs-mgbox
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Gleisbox as Command Station
  DEPENDS:=rocrail-server
endef

define Package/rocrail-cs-mgbox/description
 Using a Gleisbox as Command Station (needs CAN)
endef

define Package/rocrail-cs-ddx
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station DDX
  DEPENDS:=rocrail-server
endef

define Package/rocrail-cs-ddx/description
 Command Station DDX
endef

define Package/rocrail-cs-srcp
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station SRCP
  DEPENDS:=rocrail-server
endef

define Package/rocrail-cs-srcp/description
 Command Station SRCP
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/rocs fromtar PLATFORM=LINUX all
	cp $(PKG_BUILD_DIR)/rocs/bin/ogen $(PKG_BUILD_DIR)/rocs/bin/ogen_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/wgen $(PKG_BUILD_DIR)/rocs/bin/wgen_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/xml2cstr $(PKG_BUILD_DIR)/rocs/bin/xml2cstr_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/png2wx $(PKG_BUILD_DIR)/rocs/bin/png2wx_x86
	cp -r ./common $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		GENSUFFIX=_x86 DEBUG= CC_EXTRA_FLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -fPIC" \
		TOOLPREFIX=$(TARGET_CROSS) \
		PLATFORM=LINUX server
endef

define Package/rocrail-server/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rocrail $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/lcdriver.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/clock.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/virtual.so $(1)/opt/rocrail
endef

define Package/rocnetnode/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rocnetnode $(1)/opt/rocrail
endef

define Package/rocrail-cs-ddx/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/ddx.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-mcs2/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/mcs2.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-mgbox/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/mgbox.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-srcp/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/srcp.so $(1)/opt/rocrail
endef


$(eval $(call BuildPackage,rocrail))
$(eval $(call BuildPackage,rocrail-server))
$(eval $(call BuildPackage,rocnetnode))
$(eval $(call BuildPackage,rocrail-cs-ddx))
$(eval $(call BuildPackage,rocrail-cs-mcs2))
$(eval $(call BuildPackage,rocrail-cs-mgbox))
$(eval $(call BuildPackage,rocrail-cs-srcp))
