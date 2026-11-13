ccflags-y += -O3

# Upstream LZ4 v1.10.0 in freestanding mode (no libc, no heap).
# LZ4HC_HEAPMODE=0 up front: lz4hc.c defaults it to 1 before lz4.h's
# freestanding block can force it to 0, which would only warn.
ccflags-y += -DLZ4_FREESTANDING=1 -DLZ4HC_HEAPMODE=0 \
	'-DLZ4_memcpy(d,s,n)=__builtin_memcpy(d,s,n)' \
	'-DLZ4_memset(d,v,n)=__builtin_memset(d,v,n)' \
	'-DLZ4_memmove(d,s,n)=__builtin_memmove(d,s,n)'

# Upstream's convenience compressors share names with the kernel API
# wrappers in lz4_glue.c but have different signatures (and allocate
# ~16K state on the stack). Compile them under *_up names; the kernel
# API names are defined only by lz4_glue.c via *_extState().
ccflags-y += \
	-DLZ4_compress_fast=LZ4_compress_fast_up \
	-DLZ4_compress_default=LZ4_compress_default_up \
	-DLZ4_compress_destSize=LZ4_compress_destSize_up \
	-DLZ4_compressBound=LZ4_compressBound_up \
	-DLZ4_compress_HC=LZ4_compress_HC_up

# lz4.c carries both the compressor and the decompressor; a
# decompress-only or compress-only modular split is not supported
# (everything is =y here).
obj-$(CONFIG_LZ4_COMPRESS) += lz4.o lz4_glue.o
obj-$(CONFIG_LZ4_DECOMPRESS) += lz4.o lz4_glue.o
obj-$(CONFIG_LZ4HC_COMPRESS) += lz4.o lz4hc.o lz4_glue.o
