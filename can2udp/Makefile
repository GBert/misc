# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=can2udp
PKG_RELEASE:=1.95

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/can2udp
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=can2udp/can2lan gateway
  DEPENDS:= +zlib +libpcap
endef

define Package/can2udp/description
  can2udp/can2lan bridge CAN frames to UDP/UDP&TCP and vice versa
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

ifneq (,$(findstring bananapi,$(CONFIG_TARGET_PROFILE)))
        TARGET_CPPFLAGS+= -DSUNXI_A20
endif

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99 -g"
endef

define Package/can2udp/conffiles
/etc/init.d/can2lan
endef


define Package/can2udp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can-monitor $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can2udp $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can2lan $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/can2serial $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/clone-cs2-config $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/clone-cs2-trigger $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/clone-ms2-loco $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cs2ping $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gb2-update $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ir-tester $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lan-schnitte $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ms2wifi $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/read-cs2-ini $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/read_lococard $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pio-i2c-udp $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wake-up-c6020 $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wake-up-links88 $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xntcpd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/spitest $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init $(1)/etc/init.d/can2lan

ifneq (,$(findstring a5-v11,$(CONFIG_TARGET_PROFILE)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_slcan_a5v11 $(1)/etc/init.d/can2lan
endif

ifneq (,$(findstring banana,$(CONFIG_TARGET_PROFILE)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_bpi $(1)/etc/init.d/can2lan
	$(INSTALL_BIN) ./files/maecanserver.init $(1)/etc/init.d/maecanserver
	$(INSTALL_BIN) ./files/s88udp.init $(1)/etc/init.d/s88udp
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/s88udp-bpi $(1)/usr/bin/s88udp
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/s88test-bpi $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/s88can $(1)/usr/bin/
endif

ifneq (,$(findstring bananapro,$(CONFIG_TARGET_PROFILE)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_bpi $(1)/etc/init.d/can2lan
	$(INSTALL_BIN) ./files/maecanserver.init $(1)/etc/init.d/maecanserver
	$(INSTALL_BIN) ./files/s88udp.init_bpipro $(1)/etc/init.d/s88udp
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/s88udp-bpi $(1)/usr/bin/s88udp
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/s88test-bpi $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/s88can $(1)/usr/bin/
endif

ifneq (,$(findstring carambola,$(CONFIG_TARGET_PROFILE)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_carambola $(1)/etc/init.d/can2lan
endif

ifneq (,$(findstring mt76x8,$(CONFIG_TARGET_SUBTARGET)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_srse2_openwrt $(1)/etc/init.d/can2lan
	$(INSTALL_BIN) ./files/maecanserver.init $(1)/etc/init.d/maecanserver
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./files/no-pic.sh $(1)/root
endif

ifneq (,$(findstring mt7688,$(CONFIG_TARGET_SUBTARGET)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_srse2 $(1)/etc/init.d/can2lan
	$(INSTALL_BIN) ./files/maecanserver.init $(1)/etc/init.d/maecanserver
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./files/no-pic.sh $(1)/root
endif

ifneq (,$(findstring wr841,$(CONFIG_TARGET_PROFILE)))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/can2lan.init_slcan $(1)/etc/init.d/can2lan
endif
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./files/can2serial.init $(1)/root
	$(INSTALL_BIN) ./files/lan-schnitte.init $(1)/root/lan-schnitte
	$(INSTALL_BIN) ./files/ir.sh $(1)/root/
	$(INSTALL_BIN) ./files/card-reader.sh $(1)/root/card-reader.sh
	$(INSTALL_BIN) ./files/clone-ms2-loco.init $(1)/root/clone-ms2-loco
	$(INSTALL_BIN) ./files/pio-i2c-udp.init $(1)/root/pio-i2c-udp
	$(INSTALL_BIN) ./files/wake-up-c6020.init $(1)/root/wake-up-c6020
	$(INSTALL_BIN) ./files/wake-up-links88.init $(1)/root/wake-up-links88
#	$(INSTALL_BIN) ./files/scripts/get-icons.sh $(1)/root/get-icons.sh
	$(INSTALL_BIN) ./files/s88udp.init $(1)/root
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/usb-schnitte.sh $(1)/sbin
	$(INSTALL_DIR) $(1)/www/
	$(CP) ./files/maerklin/* $(1)/www/
	$(INSTALL_DIR) $(1)/www/betatest
	$(INSTALL_DIR) $(1)/betatest
endef

define Package/can2udp/postinst
        ln -sf /usr/bin/gb2-update $${IPKG_INSTROOT}/usr/bin/ms2-update
        ln -sf /www/config $${IPKG_INSTROOT}/www/betatest/cs2
        ln -sf /www/config $${IPKG_INSTROOT}/betatest/cs2
endef

$(eval $(call BuildPackage,can2udp))
