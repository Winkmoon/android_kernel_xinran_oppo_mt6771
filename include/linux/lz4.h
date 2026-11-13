/* LZ4 Kernel Interface
 *
 * Copyright (C) 2013, LG Electronics, Kyungsik Lee <kyungsik.lee@lge.com>
 * Copyright (C) 2016, Sven Schmidt <4sschmid@informatik.uni-hamburg.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file is based on the original header file
 * for LZ4 - Fast LZ compression algorithm.
 *
 * LZ4 - Fast LZ compression algorithm
 * Copyright (C) 2011-2024, Yann Collet.
 * BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
 *
 * The implementation in lib/lz4/ is upstream LZ4 v1.10.0
 * (https://github.com/lz4/lz4) built in freestanding mode; the wrappers
 * taking an explicit workspace live in lib/lz4/lz4_glue.c.
 *
 * Only the interfaces that in-tree code actually uses are kept: the
 * streaming/dictionary API and LZ4_compress_destSize() of the old
 * 1.7.3-era port had no users and are gone.
 */
#ifndef __LZ4_H__
#define __LZ4_H__

#include <linux/types.h>
#include <linux/string.h>	 /* memset, memcpy */

/*
 * LZ4_MEMORY_USAGE :
 * Memory usage formula : N->2^N Bytes
 * (examples : 10 -> 1KB; 12 -> 4KB ; 16 -> 64KB; 20 -> 1MB)
 * Must match LZ4_MEMORY_USAGE_DEFAULT in lib/lz4/lz4.h.
 */
#ifndef LZ4_MEMORY_USAGE
#define LZ4_MEMORY_USAGE 14
#endif

#ifndef LZ4_MAX_INPUT_SIZE
#define LZ4_MAX_INPUT_SIZE	0x7E000000 /* 2 113 929 216 bytes */
#endif
#ifndef LZ4_COMPRESSBOUND
#define LZ4_COMPRESSBOUND(isize)	(\
	(unsigned int)(isize) > (unsigned int)LZ4_MAX_INPUT_SIZE \
	? 0 \
	: (isize) + ((isize)/255) + 16)
#endif

#define LZ4_ACCELERATION_DEFAULT 1

#define LZ4HC_MIN_CLEVEL			3
#define LZ4HC_DEFAULT_CLEVEL			9
#define LZ4HC_MAX_CLEVEL			16

/*
 * Workspace sizes for the wrappers below. They must be at least
 * sizeof(LZ4_stream_t) / sizeof(LZ4_streamHC_t) of the library in
 * lib/lz4 - BUILD_BUG_ON-checked in lib/lz4/lz4_glue.c against the
 * LZ4_STREAM_MINSIZE / LZ4_STREAMHC_MINSIZE ABI constants.
 */
#define LZ4_MEM_COMPRESS	((1UL << LZ4_MEMORY_USAGE) + 32)
#define LZ4HC_MEM_COMPRESS	262200

/**
 * LZ4_compressBound() - Max. output size in worst case scenarios
 * @isize: Size of the input data
 *
 * Return: Max. size LZ4 may output in a "worst case" scenario
 *	(data not compressible)
 */
static inline int LZ4_compressBound(size_t isize)
{
	return LZ4_COMPRESSBOUND(isize);
}

/**
 * LZ4_compress_default() - Compress data from source to dest
 * @source: source address of the original data
 * @dest: output buffer address of the compressed data
 * @inputSize: size of the input data. Max supported value is
 *	LZ4_MAX_INPUT_SIZE
 * @maxOutputSize: full or partial size of buffer 'dest'
 *	which must be already allocated
 * @wrkmem: address of the working memory.
 *	This requires 'workmem' of LZ4_MEM_COMPRESS.
 *
 * Compresses 'inputSize' bytes from buffer 'source'
 * into already allocated 'dest' buffer of size 'maxOutputSize'.
 * Compression is guaranteed to succeed if
 * 'maxOutputSize' >= LZ4_compressBound(inputSize).
 * It also runs faster, so it's a recommended setting.
 * If the function cannot compress 'inputSize' bytes within 'maxOutputSize',
 * compression stops *immediately*, and the function result is zero.
 * As a consequence, 'dest' content is not valid.
 *
 * Return: Number of bytes written into buffer 'dest'
 *	(necessarily <= maxOutputSize) or 0 if compression fails
 */
int LZ4_compress_default(const char *source, char *dest, int inputSize,
	int maxOutputSize, void *wrkmem);

/**
 * LZ4_compress_fast() - As LZ4_compress_default providing an acceleration
 *	param
 * @source: source address of the original data
 * @dest: output buffer address of the compressed data
 * @inputSize: size of the input data. Max supported value is
 *	LZ4_MAX_INPUT_SIZE
 * @maxOutputSize: full or partial size of buffer 'dest'
 *	which must be already allocated
 * @acceleration: acceleration factor
 * @wrkmem: address of the working memory.
 *	This requires 'workmem' of LZ4_MEM_COMPRESS.
 *
 * Same as LZ4_compress_default(), but allows to select an "acceleration"
 * factor. The larger the acceleration value, the faster the algorithm,
 * but also the lesser the compression. It's a trade-off. It can be fine
 * tuned, with each successive value providing roughly +~3% to speed.
 * An acceleration value of "1" is the same as regular
 * LZ4_compress_default(). Values <= 0 will be replaced by
 * LZ4_ACCELERATION_DEFAULT, which is 1.
 *
 * Return: Number of bytes written into buffer 'dest'
 *	(necessarily <= maxOutputSize) or 0 if compression fails
 */
int LZ4_compress_fast(const char *source, char *dest, int inputSize,
	int maxOutputSize, int acceleration, void *wrkmem);

/**
 * LZ4_compress_HC() - Compress data from source to dest using high
 *	compression ratio
 * @src: source address of the original data
 * @dst: output buffer address of the compressed data
 * @srcSize: size of the input data. Max supported value is
 *	LZ4_MAX_INPUT_SIZE
 * @dstCapacity: full or partial size of buffer 'dst',
 *	which must be already allocated
 * @compressionLevel: Recommended values are between 4 and 9, although any
 *	value between 1 and LZ4HC_MAX_CLEVEL will work.
 *	Values > LZ4HC_MAX_CLEVEL behave the same as 16.
 * @wrkmem: address of the working memory.
 *	This requires 'workmem' of size LZ4HC_MEM_COMPRESS.
 *
 * Transform buffer @src into a compressed buffer @dst using high compression.
 *
 * Return: Number of bytes written into buffer 'dst'
 *	(necessarily <= dstCapacity) or 0 if compression fails
 */
int LZ4_compress_HC(const char *src, char *dst, int srcSize, int dstCapacity,
	int compressionLevel, void *wrkmem);

/**
 * LZ4_decompress_fast() - Decompresses data from 'source' into 'dest'
 * @source: source address of the compressed data
 * @dest: output buffer address of the uncompressed data
 *	which must be already allocated with 'originalSize' bytes
 * @originalSize: is the original and therefore uncompressed size
 *
 * This function is deprecated upstream (it cannot bound its reads from
 * 'source'); do not add new callers - use LZ4_decompress_safe() instead.
 * Kept for lib/decompress_unlz4.c.
 *
 * Return: number of bytes read from the source buffer
 *	or a negative result if decompression fails.
 */
int LZ4_decompress_fast(const char *source, char *dest, int originalSize);

/**
 * LZ4_decompress_safe() - Decompression protected against buffer overflow
 * @source: source address of the compressed data
 * @dest: output buffer address of the uncompressed data
 *	which must be already allocated
 * @compressedSize: is the precise full size of the compressed block
 * @maxDecompressedSize: is the size of 'dest' buffer
 *
 * Decompresses data from 'source' into 'dest'.
 * If the source stream is detected malformed, the function will
 * stop decoding and return a negative result.
 * This function is protected against buffer overflow exploits,
 * including malicious data packets. It never writes outside output buffer,
 * nor reads outside input buffer.
 *
 * Return: number of bytes decompressed into destination buffer
 *	(necessarily <= maxDecompressedSize)
 *	or a negative result in case of error
 */
int LZ4_decompress_safe(const char *source, char *dest, int compressedSize,
	int maxDecompressedSize);

/**
 * LZ4_decompress_safe_partial() - Decompress a block of size 'compressedSize'
 *	at position 'source' into buffer 'dest'
 * @source: source address of the compressed data
 * @dest: output buffer address of the decompressed data which must be
 *	already allocated
 * @compressedSize: is the precise full size of the compressed block.
 * @targetOutputSize: the decompression operation will try
 *	to stop as soon as 'targetOutputSize' has been reached
 * @dstCapacity: is the size of destination buffer
 *	(which must be already allocated), presumed an upper bound of
 *	decompressed size.
 *
 * Note: since LZ4 1.9 decoding stops as soon as 'targetOutputSize' is
 * reached; older versions could decode more. Callers must not assume
 * anything past 'targetOutputSize' is valid output.
 *
 * Return: the number of bytes decoded in the destination buffer
 *	(necessarily <= dstCapacity)
 *	or a negative result in case of error
 */
int LZ4_decompress_safe_partial(const char *source, char *dest,
	int compressedSize, int targetOutputSize, int dstCapacity);

#endif
