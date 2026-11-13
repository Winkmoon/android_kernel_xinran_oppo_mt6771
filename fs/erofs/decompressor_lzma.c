// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * EROFS LZMA decompression (microLZMA).
 *
 * LZMA is optional and rarely used on this tree, so a single serialized
 * decoder is kept instead of a per-CPU pool: the memory footprint stays
 * predictable and there is no contention worth optimizing for.
 */
#include <linux/xz.h>
#include <linux/module.h>
#include "compress.h"

static DEFINE_MUTEX(z_erofs_lzma_mutex);
static struct xz_dec_microlzma *z_erofs_lzma_state;
static unsigned int z_erofs_lzma_dictsize = 65536;
static u8 *z_erofs_lzma_inbuf;
static unsigned int z_erofs_lzma_inbuf_size;

int z_erofs_load_lzma_config(struct super_block *sb,
			     struct erofs_super_block *dsb,
			     struct z_erofs_lzma_cfgs *lzma, int size)
{
	unsigned int dict_size;

	if (!lzma || size < sizeof(struct z_erofs_lzma_cfgs)) {
		erofs_err(sb, "invalid lzma cfgs, size=%u", size);
		return -EINVAL;
	}
	if (lzma->format) {
		erofs_err(sb, "unidentified lzma format %x, please check kernel version",
			  le16_to_cpu(lzma->format));
		return -EINVAL;
	}

	dict_size = le32_to_cpu(lzma->dict_size);
	if (dict_size > Z_EROFS_LZMA_MAX_DICT_SIZE || dict_size < 4096) {
		erofs_err(sb, "invalid lzma dict size %u", dict_size);
		return -EINVAL;
	}

	mutex_lock(&z_erofs_lzma_mutex);
	if (z_erofs_lzma_dictsize != dict_size) {
		z_erofs_lzma_dictsize = dict_size;
		if (z_erofs_lzma_state) {
			xz_dec_microlzma_end(z_erofs_lzma_state);
			z_erofs_lzma_state = NULL;
		}
	}
	mutex_unlock(&z_erofs_lzma_mutex);
	return 0;
}

/*
 * Unlike the LZ4 one this does not try to reuse pages within the LZ4
 * dictionary window: an LZMA image never loads the LZ4 configuration, so
 * EROFS_SB(sb)->lz4.max_distance_pages is zero here.
 */
int z_erofs_lzma_prepare_destpages(struct z_erofs_decompress_req *rq,
				   struct list_head *pagepool)
{
	const unsigned int nr =
		PAGE_ALIGN(rq->pageofs_out + rq->outputsize) >> PAGE_SHIFT;
	void *kaddr = NULL;
	unsigned int i;

	rq->inplace_io = false;
	for (i = 0; i < nr; ++i) {
		struct page *page = rq->out[i];

		if (!page) {
			page = erofs_allocpage(pagepool,
					       GFP_KERNEL | __GFP_NOFAIL);
			set_page_private(page, Z_EROFS_SHORTLIVED_PAGE);
			rq->out[i] = page;
			kaddr = NULL;
			continue;
		}
		if (!PageHighMem(page)) {
			if (!i) {
				kaddr = page_address(page);
				continue;
			}
			if (kaddr && kaddr + PAGE_SIZE == page_address(page)) {
				kaddr += PAGE_SIZE;
				continue;
			}
		}
		kaddr = NULL;
	}
	return kaddr ? 1 : 0;
}

static int z_erofs_lzma_prepare(void)
{
	if (z_erofs_lzma_state)
		return 0;
	z_erofs_lzma_state = xz_dec_microlzma_alloc(XZ_PREALLOC,
						   z_erofs_lzma_dictsize);
	return z_erofs_lzma_state ? 0 : -ENOMEM;
}

int z_erofs_lzma_decompress(struct z_erofs_decompress_req *rq, u8 *dst)
{
	const unsigned int inlen = rq->inputsize;
	const unsigned int outlen = rq->outputsize;
	const unsigned int nrpages_in = DIV_ROUND_UP(inlen, PAGE_SIZE);
	struct xz_buf buf;
	enum xz_ret ret;
	unsigned int i, pad;
	int err = 0;

	if (!inlen || !outlen)
		return -EINVAL;

	mutex_lock(&z_erofs_lzma_mutex);
	err = z_erofs_lzma_prepare();
	if (err)
		goto out_unlock;

	/* linearize the compressed input (a pcluster is at most 1 MiB) */
	if (z_erofs_lzma_inbuf_size < inlen) {
		u8 *nb = kmalloc(inlen, GFP_NOFS);

		if (!nb) {
			err = -ENOMEM;
			goto out_unlock;
		}
		kfree(z_erofs_lzma_inbuf);
		z_erofs_lzma_inbuf = nb;
		z_erofs_lzma_inbuf_size = inlen;
	}

	for (i = 0; i < nrpages_in; ++i) {
		const unsigned int off = i << PAGE_SHIFT;
		const unsigned int n = min_t(unsigned int, PAGE_SIZE, inlen - off);
		u8 *p = kmap_atomic(rq->in[i]);

		memcpy(z_erofs_lzma_inbuf + off, p, n);
		kunmap_atomic(p);
	}

	/*
	 * The stream is right-aligned in the cluster: skip the leading zero
	 * padding before handing it to the decoder (same as 0padding for LZ4).
	 */
	pad = 0;
	while (pad < inlen && !z_erofs_lzma_inbuf[pad])
		++pad;
	if (pad == inlen) {
		err = -EFSCORRUPTED;
		goto out_unlock;
	}

	memset(&buf, 0, sizeof(buf));
	buf.in = z_erofs_lzma_inbuf + pad;
	buf.in_pos = 0;
	buf.in_size = inlen - pad;
	buf.out = dst + rq->pageofs_out;
	buf.out_pos = 0;
	buf.out_size = outlen;

	xz_dec_microlzma_reset(z_erofs_lzma_state, inlen - pad, outlen,
			       !rq->partial_decoding);
	for (;;) {
		ret = xz_dec_microlzma_run(z_erofs_lzma_state, &buf);
		if (ret == XZ_STREAM_END)
			break;
		if (ret != XZ_OK) {
			erofs_err(rq->sb, "failed to decompress %d in[%u, %u] out[%u]",
				  ret, inlen, pad, outlen);
			err = -EFSCORRUPTED;
			break;
		}
	}

	if (!err && buf.out_pos != outlen)
		err = -EFSCORRUPTED;

out_unlock:
	mutex_unlock(&z_erofs_lzma_mutex);
	return err;
}
