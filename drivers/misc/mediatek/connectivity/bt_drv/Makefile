MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))

ifndef AUTOCONF_H
$(error AUTOCONF_H is not defined)
endif
ccflags-y += -imacros $(AUTOCONF_H)

ifndef TOP
TOP := $(srctree)/..
endif

ifneq ($(CFG_BT_PM_QOS_CONTROL),)
ccflags-y += -D PM_QOS_CONTROL=1
else
ccflags-y += -D PM_QOS_CONTROL=0
endif

ifneq ($(CFG_BT_READ_MAILBOX),)
ccflags-y += -D READ_MAILBOX=1
else
ccflags-y += -D READ_MAILBOX=0
endif

ccflags-y += -D MTK_WCN_WMT_STP_EXP_SYMBOL_ABSTRACT
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include

ifeq ($(CONFIG_MTK_CONN_LTE_IDC_SUPPORT),y)
ccflags-y += -D WMT_IDC_SUPPORT=1
else
ccflags-y += -D WMT_IDC_SUPPORT=0
endif

# Built-in: WMT headers come from the in-tree wmt_drv, same as gen3.
# MTK_WCN_REMOVE_KERNEL_MODULE is supplied by the parent connectivity Makefile.
WMT_SRC_FOLDER := $(srctree)/drivers/misc/mediatek/connectivity/wmt_drv
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/include
ccflags-y += -I$(WMT_SRC_FOLDER)/common_main/linux/include
ifneq ($(CONFIG_MTK_CONNSYS_DEDICATED_LOG_PATH),)
ccflags-y += -I$(WMT_SRC_FOLDER)/debug_utility
endif

MODULE_NAME := bt_drv
obj-y += $(MODULE_NAME).o

ccflags-y += -D CREATE_NODE_DYNAMIC=1

$(MODULE_NAME)-objs += stp_chrdev_bt.o
$(MODULE_NAME)-objs += dbg_bt.o
ifneq ($(CONFIG_MTK_CONNSYS_DEDICATED_LOG_PATH),)
$(MODULE_NAME)-objs += fw_log_bt.o
endif
