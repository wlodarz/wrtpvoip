#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ar7voip
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ar7voip
  SUBMENU:=AR7VOIP support
  TITLE:=AR7VOIP subsystem support
  DEPENDS:=
  FILES:=$(PKG_BUILD_DIR)/ar7dsp.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,55,ar7voip)
endef

define KernelPackage/ar7voip/description
 Kernel module to register a ar7voip platform device.
endef

EXTRA_KCONFIG:= \
	CONFIG_AR7VOIP=m

EXTRA_CFLAGS:= \
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
	echo Preparing
endef

define Build/Compile
	echo Compiling
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,ar7voip))

