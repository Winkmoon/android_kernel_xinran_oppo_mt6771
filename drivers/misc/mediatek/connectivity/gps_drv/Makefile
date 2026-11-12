MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))

ifndef AUTOCONF_H
$(error AUTOCONF_H is not defined)
endif
ccflags-y += -imacros $(AUTOCONF_H)

ifndef TOP
TOP := $(srctree)/..
endif

ifeq ($(CONFIG_ARM64), y)
ccflags-y += -D CONFIG_MTK_WCN_ARM64
endif

ifeq ($(CONFIG_MTK_CONN_LTE_IDC_SUPPORT),y)
ccflags-y += -D WMT_IDC_SUPPORT=1
else
ccflags-y += -D WMT_IDC_SUPPORT=0
endif
ccflags-y += -D MTK_WCN_WMT_STP_EXP_SYMBOL_ABSTRACT

ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/mach
ccflags-y += -I$(srctree)/drivers/misc/mediatek/freqhopping
ccflags-y += -I$(srctree)/drivers/misc/mediatek/freqhopping/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/emi/submodule
ccflags-y += -I$(srctree)/drivers/misc/mediatek/emi/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/common
ccflags-y += -I$(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/include/mach
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include/clkbuf_v1
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include/clkbuf_v1/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/devfreq

MODULE_NAME := gps_drv
obj-$(CONFIG_MTK_GPS_SUPPORT) += $(MODULE_NAME).o

# Built-in: WMT headers come from the in-tree wmt_drv.
# MTK_WCN_REMOVE_KERNEL_MODULE is supplied by the parent connectivity Makefile.
WMT_SRC_FOLDER := $(srctree)/drivers/misc/mediatek/connectivity/wmt_drv

ifneq ($(filter "CONSYS_%",$(CONFIG_MTK_COMBO_CHIP)),)
ccflags-y += -DSOC_CO_CLOCK_FLAG=1
ccflags-y += -DWMT_CREATE_NODE_DYNAMIC=1
ccflags-y += -DREMOVE_MK_NODE=0
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/$(MTK_PLATFORM)/include
else
ccflags-y += -DSOC_CO_CLOCK_FLAG=0
ccflags-y += -DWMT_CREATE_NODE_DYNAMIC=0
ccflags-y += -DREMOVE_MK_NODE=1
endif

ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/linux/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/core/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/platform/include
ifneq ($(CONFIG_MTK_CONNSYS_DEDICATED_LOG_PATH),)
ccflags-y += -I$(WMT_SRC_FOLDER)/debug_utility
endif

ifeq ($(CONFIG_MTK_CONN_MT3337_CHIP_SUPPORT),y)
$(MODULE_NAME)-objs += gps_mt3337.o
else
$(MODULE_NAME)-objs += stp_chrdev_gps.o
endif
ifneq ($(CONFIG_MTK_GPS_EMI),)
$(MODULE_NAME)-objs += gps_emi.o
endif
ifneq ($(CONFIG_MTK_CONNSYS_DEDICATED_LOG_PATH),)
$(MODULE_NAME)-objs += fw_log_gps.o
endif
