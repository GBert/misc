# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rocrail
PKG_REV:=82d967e0ae
PKG_VERSION:=n783
PKG_RELEASE:=

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

define Package/rocnetnode
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=rocnetnode
  DEPENDS:=+libpthread +rocrail
endef

define Package/rocnetnode/description
 This package contains the rocnetnode
endef

define Package/rocweb
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=rocweb
  DEPENDS:=+libpthread +rocrail
endef

define Package/rocweb/description
 This package contains Rocweb, a WEB Browser based Rocrail client for all platforms including mobile devices
endef

define Package/rocrail-mqtt
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=MQTT extension
  DEPENDS:=+libpthread +rocrail
endef

define Package/rocrail-mqtt/description
 MQTT extension for Rocrail
endef

define Package/rocrail-cs-mbus
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Maerklin CS2/MS2 CAN-Bus (e.g. SocketCAN)  
  DEPENDS:=rocrail
endef

define Package/rocrail-cs-mbus/description
 Maerklin CS2/MS2 CAN-Bus (e.g. SocketCAN)
endef

define Package/rocrail-cs-p50
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station p50
  DEPENDS:=rocrail
endef

define Package/rocrail-cs-p50/description
 Command Station p50
endef

define Package/rocrail-cs-srcp
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station SRCP
  DEPENDS:=rocrail
endef

define Package/rocrail-cs-srcp/description
 Command Station SRCP
endef

define Package/rocrail-cs-rclink
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station RCLink
  DEPENDS:=rocrail
endef

define Package/rocrail-cs-rclink/description
 Command Station RCLink
endef

define Package/rocrail-cs-wmz21
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Command Station WLAN Mouse Z21
  DEPENDS:=rocrail
endef

define Package/rocrail-cs-wmz21/description
 Virtual Z21 for Roco Wlan Mouse
endef

define Package/rocrail-cs-all-other
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=All other Command Stations
  DEPENDS:=+libusb-1.0 +rocrail
endef

define Package/rocrail-cs-all-other/description
 All Command Stations
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/rocs fromtar PLATFORM=LINUX all
	cp $(PKG_BUILD_DIR)/rocs/bin/ogen $(PKG_BUILD_DIR)/rocs/bin/ogen_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/wgen $(PKG_BUILD_DIR)/rocs/bin/wgen_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/xml2cstr $(PKG_BUILD_DIR)/rocs/bin/xml2cstr_x86
	cp $(PKG_BUILD_DIR)/rocs/bin/png2wx $(PKG_BUILD_DIR)/rocs/bin/png2wx_x86
	cp -r ./common $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		GENSUFFIX=_x86 DEBUG= CC_EXTRA_FLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -fPIC -D__NO_DIRECT_IO__" \
		TOOLPREFIX=$(TARGET_CROSS) CPP=$(TARGET_CROSS)gcc \
		PLATFORM=LINUX server
endef

define Package/rocrail/conffiles
/opt/rocrail/plan.xml
/opt/rocrail/rocnetnode.ini
/opt/rocrail/rocrail.ini
endef

define Package/rocrail/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/canservice $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/clock.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/lcdriver.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rocabox.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rocrail $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rocnet.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/virtual.so $(1)/opt/rocrail
	$(INSTALL_BIN) ./config/rocrail.ini $(1)/opt/rocrail
	$(INSTALL_BIN) ./config/plan.xml $(1)/opt/rocrail
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./config/rocrail.init $(1)/root/rocrail
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./config/rocrail.init $(1)/etc/init.d/rocrail
endef

define Package/rocnetnode/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rocnetnode $(1)/opt/rocrail
	$(INSTALL_BIN) ./config/rocnetnode.ini $(1)/opt/rocrail
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./config/rocnetnode.init $(1)/root/rocnetnode
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./config/rocnetnode.init $(1)/etc/init.d/rocnetnode
endef

define Package/rocweb/install
	$(INSTALL_DIR) $(1)/opt/rocrail/svg
	$(INSTALL_DIR) $(1)/opt/rocrail/images
	$(INSTALL_DIR) $(1)/opt/rocrail/web
	$(INSTALL_DIR) $(1)/opt/rocrail/web/html
	$(CP) $(PKG_BUILD_DIR)/rocview/svg/* $(1)/opt/rocrail/svg
	$(CP) $(PKG_BUILD_DIR)/rocrail/impl/web/html/* $(1)/opt/rocrail/web/html
endef

define Package/rccrail-mqtt/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/mqtt.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-mbus/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/mbus.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-p50/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/p50.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-rclink/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rclink.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-srcp/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/srcp.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-wmz21/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/wmz21.so $(1)/opt/rocrail
endef

define Package/rocrail-cs-all-other/install
	$(INSTALL_DIR) $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/barjut.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/bidib.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/cbus.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/cti.so $(1)/opt/rocrail
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dcc232.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dccar.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dccpp.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dinamo.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dmx4all.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dmxartnet.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dmxeurolite.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/dumper.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/easydcc.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/ecos.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/editspro.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/esunavi.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/got.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/hsi88.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/hue.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/infracar.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/inter10.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/lan.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/lcm.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/loconet.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/massoth.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/mttmfcc.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/muet.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/nce.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/om32.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/opendcc.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/perir.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/raptor.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rascii.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rfid12.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/rmx.so $(1)/opt/rocrail
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/roco.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/slx.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/spl.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/sprog.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/tamsmc.so $(1)/opt/rocrail
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/xpressnet.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/z21.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/zimobin.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/zimocan.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/zimo.so $(1)/opt/rocrail
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/unxbin/zs2.so $(1)/opt/rocrail
endef


$(eval $(call BuildPackage,rocrail))
$(eval $(call BuildPackage,rocnetnode))
$(eval $(call BuildPackage,rocweb))
$(eval $(call BuildPackage,rocrail-mqtt))
$(eval $(call BuildPackage,rocrail-cs-mbus))
$(eval $(call BuildPackage,rocrail-cs-p50))
$(eval $(call BuildPackage,rocrail-cs-rclink))
$(eval $(call BuildPackage,rocrail-cs-srcp))
$(eval $(call BuildPackage,rocrail-cs-wmz21))
$(eval $(call BuildPackage,rocrail-cs-all-other))
