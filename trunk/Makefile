#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=titanvoip
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/titanvoip
  SUBMENU:=TitanVOIP support
  TITLE:=TitanVOIP subsystem support
  DEPENDS:=
  FILES:= \
  	$(PKG_BUILD_DIR)/titan_dsp.$(LINUX_KMOD_SUFFIX) \
  	$(PKG_BUILD_DIR)/titan_tiuhal.$(LINUX_KMOD_SUFFIX) \
  	$(PKG_BUILD_DIR)/titan_mxpmod.$(LINUX_KMOD_SUFFIX) \
  	$(PKG_BUILD_DIR)/titan_hwan.$(LINUX_KMOD_SUFFIX) \
  	$(PKG_BUILD_DIR)/legerity/le88x.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,55,titan_mxpmod titan_tiuhal titan_dsp titan_hwan le88x testapp)
endef

define KernelPackage/titanvoip/description
 Kernel module to register a titan_voip platform device.
endef

EXTRA_KCONFIG:= \
	CONFIG_TITANVOIP=m

EXTRA_CFLAGS:= -O0 \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) 
	

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) -r ./apps/ $(PKG_BUILD_DIR)/
	echo Preparing
endef

define Build/Compile
	echo Compiling
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
	$(MAKE) -C "$(PKG_BUILD_DIR)/apps/"
endef

$(eval $(call KernelPackage,titanvoip))

