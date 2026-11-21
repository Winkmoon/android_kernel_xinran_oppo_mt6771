// SPDX-License-Identifier: GPL-2.0
/*
 * Glue between the kernel LZ4 API (include/linux/lz4.h) and upstream
 * LZ4 v1.10.0 in this directory.
 *
 * The kernel API passes an explicit workspace ('wrkmem') to the
 * compressors, which maps 1:1 onto upstream's public *_extState()
 * entry points. The decompressors have identical signatures upstream,
 * so their definitions in lz4.c are used directly and only exported
 * here.
 *
 * Upstream also defines stack/heap-state convenience compressors under
 * the same names as the kernel wrappers but with different signatures
 * (and a ~16K stack footprint). Those are compiled under *_up names -
 * see the -D renames in the Makefile, undone below before including
 * the kernel header - so no wrong-ABI symbol exists under a kernel API
 * name.
 */
#define LZ4_STATIC_LINKING_ONLY
#define LZ4_HC_STATIC_LINKING_ONLY
#include "lz4.h"
#include "lz4hc.h"

#undef LZ4_compress_fast
#undef LZ4_compress_default
#undef LZ4_compress_destSize
#undef LZ4_compressBound
#undef LZ4_compress_HC

#include <linux/lz4.h>
#include <linux/kernel.h>
#include <linux/module.h>

static void __maybe_unused lz4_abi_checks(void)
{
	BUILD_BUG_ON(LZ4_MEM_COMPRESS < sizeof(LZ4_stream_t));
	BUILD_BUG_ON(LZ4_MEM_COMPRESS < LZ4_STREAM_MINSIZE);
	BUILD_BUG_ON(LZ4HC_MEM_COMPRESS < sizeof(LZ4_streamHC_t));
	BUILD_BUG_ON(LZ4HC_MEM_COMPRESS < LZ4_STREAMHC_MINSIZE);
}

#ifdef CONFIG_LZ4_COMPRESS
int LZ4_compress_fast(const char *source, char *dest, int inputSize,
	int maxOutputSize, int acceleration, void *wrkmem)
{
	return LZ4_compress_fast_extState(wrkmem, source, dest, inputSize,
					  maxOutputSize, acceleration);
}
EXPORT_SYMBOL(LZ4_compress_fast);

int LZ4_compress_default(const char *source, char *dest, int inputSize,
	int maxOutputSize, void *wrkmem)
{
	return LZ4_compress_fast_extState(wrkmem, source, dest, inputSize,
					  maxOutputSize,
					  LZ4_ACCELERATION_DEFAULT);
}
EXPORT_SYMBOL(LZ4_compress_default);
#endif /* CONFIG_LZ4_COMPRESS */

#ifdef CONFIG_LZ4HC_COMPRESS
int LZ4_compress_HC(const char *src, char *dst, int srcSize, int dstCapacity,
	int compressionLevel, void *wrkmem)
{
	/*
	 * Levels >= LZ4HC_CLEVEL_OPT_MIN run LZ4HC_compress_optimal, whose
	 * ~64 KB opt[] frame (LZ4HC_HEAPMODE=0, no heap in freestanding
	 * mode) cannot fit a 16 KB kernel stack. Cap at the highest
	 * non-optimal level; in-tree callers only use level 9 anyway.
	 */
	if (compressionLevel >= LZ4HC_CLEVEL_OPT_MIN)
		compressionLevel = LZ4HC_CLEVEL_OPT_MIN - 1;

	return LZ4_compress_HC_extStateHC(wrkmem, src, dst, srcSize,
					  dstCapacity, compressionLevel);
}
EXPORT_SYMBOL(LZ4_compress_HC);
#endif /* CONFIG_LZ4HC_COMPRESS */

/* Defined in lz4.c with kernel-identical signatures. */
EXPORT_SYMBOL(LZ4_decompress_safe);
EXPORT_SYMBOL(LZ4_decompress_safe_partial);
EXPORT_SYMBOL(LZ4_decompress_fast);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("LZ4 compressor/decompressor (upstream v1.10.0)");
