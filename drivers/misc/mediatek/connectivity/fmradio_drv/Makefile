ifndef AUTOCONF_H
$(error AUTOCONF_H is not defined)
endif
ccflags-y += -imacros $(AUTOCONF_H)

MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))

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
ccflags-y += -I$(srctree)/drivers/misc/mediatek/connectivity/common

MODULE_NAME := fmradio_drv
obj-$(CONFIG_MTK_FMRADIO) += $(MODULE_NAME).o

# MT6771 uses the mt6631 FM library.  MTK_WCN_REMOVE_KERNEL_MODULE is
# supplied by the parent connectivity Makefile, so fm_module.c exports
# mtk_wcn_fm_init() instead of registering a module initcall.
FM_CHIP := mt6631
ccflags-y += -DMT6631_FM
ccflags-y += -DCFG_FM_CONNAC2=0
ccflags-y += -I$(src)/chips/mt6631/inc
ccflags-y += -I$(src)/inc
ccflags-y += -I$(src)/plat/inc

FM_OBJS := chips/mt6631/pub/mt6631_fm_rds.o
FM_OBJS += chips/mt6631/pub/mt6631_fm_lib.o
FM_OBJS += core/fm_module.o
FM_OBJS += core/fm_main.o
FM_OBJS += core/fm_config.o
FM_OBJS += core/fm_rds_parser.o
FM_OBJS += core/fm_patch.o
FM_OBJS += core/fm_utils.o
FM_OBJS += core/fm_link.o
FM_OBJS += core/fm_eint.o
FM_OBJS += core/fm_cmd.o
FM_OBJS += core/fm_reg_utils.o
FM_OBJS += plat/legacy_wmt.o

$(MODULE_NAME)-objs += $(FM_OBJS)

# In-tree WMT headers, matching the other connectivity modules.
WMT_SRC_FOLDER := $(srctree)/drivers/misc/mediatek/connectivity/wmt_drv
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/linux/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/core/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/platform/include
