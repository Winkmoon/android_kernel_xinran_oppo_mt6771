subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
subdir-ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
subdir-ccflags-y += -I$(srctree)/drivers/spi/trustzone/spi/Tlspi/inc

ifneq ($(filter kernel-4.14 kernel-4.9, $(LINUX_KERNEL_VERSION)),)
include $(srctree)/drivers/misc/mediatek/gud/Makefile.include
endif

ccflags-y += -DCONFIG_MTK_DRIVER
obj-y    += et713_mtk.o et713_netlink.o
