DEFS = -DCONFIG_OPPO_FINGERPRINT_KERNEL_VER=$(VERSION)
ccflags-y += $(DEFS)

subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
subdir-ccflags-y += -I$(srctree)/drivers/spi

obj-y  += fpc_irq.o
