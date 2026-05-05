#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

# Connectivity combo driver
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
    subdir-ccflags-y += -I$(srctree)/
    subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
    subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include/clkbuf_v1
    subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include/clkbuf_v1/$(MTK_PLATFORM)
    subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
    subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ifeq ($(CONFIG_MTK_PMIC_CHIP_MT6359),y)
    subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/pmic/include/mt6359
endif
ifeq ($(CONFIG_MTK_PMIC_NEW_ARCH),y)
    subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/pmic/include
endif

ifdef CONFIG_OPLUS_FEATURE_MMCPLUS
MMC_BUILD_FOLDER :=ommc
else
MMC_BUILD_FOLDER :=mmc
endif
    subdir-ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/core
    subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/$(MTK_PLATFORM)
    subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/
    subdir-ccflags-y += -I$(srctree)/drivers/clk/mediatek/
    subdir-ccflags-y += -I$(srctree)/drivers/pinctrl/mediatek/

    # Do Nothing, move to standalone repo
    MODULE_NAME := connadp
    obj-y += $(MODULE_NAME).o
    $(MODULE_NAME)-objs += common/connectivity_build_in_adapter.o
    $(MODULE_NAME)-objs += common/wmt_build_in_adapter.o


    # WiFi built-in for CONSYS_6771 (source in-tree, no vendor dependency)
    export CONFIG_WLAN_DRV_BUILD_IN=y
    export MTK_COMBO_CHIP=CONNAC
    MTK_PLATFORM_ID := $(patsubst CONSYS_%,%,$(subst ",,$(CONFIG_MTK_COMBO_CHIP)))
    $(info MTK_PLATFORM_ID is [${MTK_PLATFORM_ID}])

    export CONFIG_MTK_COMBO_WIFI_HIF=axi
    export WLAN_CHIP_ID=$(MTK_PLATFORM_ID)
    export MTK_ANDROID_WMT=y
    export MTK_ANDROID_EMI=y
    export ADAPTOR_OPTS=CONNAC
    export WIFI_IP_SET=1

    subdir-ccflags-y += -D MTK_WCN_REMOVE_KERNEL_MODULE
    subdir-ccflags-y += -D MTK_WCN_BUILT_IN_DRIVER
    obj-y += wmt_drv/
    obj-y += wmt_chrdev_wifi/
    obj-y += wlan_drv_gen4m/

        # For BT built-in mode start @{
        # ifneq (,$(filter $(CONFIG_MTK_COMBO_CHIP), "CONSYS_6885"))
        #     PATH_TO_BT_DRV       = vendor/mediatek/kernel_modules/connectivity/bt/mt66xx/connac2
        #     export _MTK_BT_CHIP=MTK_CONSYS_MT6885
        # else ifneq (,$(filter $(CONFIG_MTK_COMBO_CHIP), "CONSYS_6893"))
        #     PATH_TO_BT_DRV       = vendor/mediatek/kernel_modules/connectivity/bt/mt66xx/connac2
        #     export _MTK_BT_CHIP=MTK_CONSYS_MT6893
        # else
        #     PATH_TO_BT_DRV       = vendor/mediatek/kernel_modules/connectivity/bt/mt66xx/legacy
        # endif
        # ABS_PATH_TO_BT_DRV       = $(srctree)/../$(PATH_TO_BT_DRV)
        # $(shell rm -rf $(srctree)/$(src)/bt)
        # $(shell cp -r $(ABS_PATH_TO_BT_DRV)      $(srctree)/$(src)/bt)
        # obj-y += bt/
        # @} For BT built-in mode end


        # For FM built-in mode start @{
        # for fmradio options
        # ifneq (,$(filter $(CONFIG_MTK_COMBO_CHIP), "CONSYS_6885" "CONSYS_6893"))
        #     export CFG_BUILD_CONNAC2=true
        # else
        #     export CFG_BUILD_CONNAC2=false
        # endif
        # FM_6631_CHIPS := 6758 6759 6771 6775 6765 6761 3967 6797 6768 6785 8168
        # FM_6635_CHIPS := 6779 6885 6873 6893
        # ifneq ($(filter $(FM_6631_CHIPS), $(MTK_PLATFORM_ID)),)
        #     FM_CHIP := mt6631
        # else ifneq ($(filter $(FM_6635_CHIPS), $(MTK_PLATFORM_ID)),)
        #     FM_CHIP := mt6635
        # endif
        # export CFG_FM_CHIP_ID=$(MTK_PLATFORM_ID)
        # export CFG_FM_CHIP=$(FM_CHIP)
        # PATH_TO_FMRADIO_DRV       = vendor/mediatek/kernel_modules/connectivity/fmradio
        # ABS_PATH_TO_FMRADIO_DRV  = $(srctree)/../$(PATH_TO_FMRADIO_DRV)
        # $(shell rm -rf $(srctree)/$(src)/fmradio)
        # $(shell cp -r $(ABS_PATH_TO_FMRADIO_DRV)      $(srctree)/$(src)/fmradio)
        # obj-y += fmradio/
        # @} For FM built-in mode end

        # For GPS built-in mode start @{
        # PATH_TO_GPS_DRV      = vendor/mediatek/kernel_modules/connectivity/gps
        # ABS_PATH_TO_GPS_DRV      = $(srctree)/../$(PATH_TO_GPS_DRV)
        # ifeq (,$(wildcard $(ABS_PATH_TO_GPS_DRV)))
        #     $(error $(ABS_PATH_TO_GPS_DRV) is not existed)
        # endif
        # $(warning copying from $(PATH_TO_GPS_DRV))
        # $(shell rm -rf $(srctree)/$(src)/gps_drv)
        # $(shell cp -r $(ABS_PATH_TO_GPS_DRV)      $(srctree)/$(src)/gps_drv)
        # obj-y += gps_drv/
        # @} For GPS built-in mode end



# Otherwise we were called directly from the command line;
# invoke the kernel build system.
else
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
    PWD  := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
