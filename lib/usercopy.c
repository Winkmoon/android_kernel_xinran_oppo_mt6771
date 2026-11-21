// SPDX-License-Identifier: GPL-2.0
#include <linux/uaccess.h>
#include <linux/nospec.h>

/* out-of-line parts */

#ifndef INLINE_COPY_FROM_USER
unsigned long _copy_from_user(void *to, const void __user *from, unsigned long n)
{
	unsigned long res = n;
	might_fault();
	if (likely(access_ok(VERIFY_READ, from, n))) {
		/*
		 * Ensure that bad access_ok() speculation will not
		 * lead to nasty side effects *after* the copy is
		 * finished:
		 */
		barrier_nospec();
		kasan_check_write(to, n);
		res = raw_copy_from_user(to, from, n);
	}
	if (unlikely(res))
		memset(to + (n - res), 0, res);
	return res;
}
EXPORT_SYMBOL(_copy_from_user);
#endif

#ifndef INLINE_COPY_TO_USER
unsigned long _copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	if (likely(access_ok(VERIFY_WRITE, to, n))) {
		kasan_check_read(from, n);
		n = raw_copy_to_user(to, from, n);
	}
	return n;
}
EXPORT_SYMBOL(_copy_to_user);
#endif

int check_zeroed_user(const void __user *from, size_t size)
{
	const u8 __user *p = from;
	u8 val;

	if (unlikely(size == 0))
		return 1;

	while (size--) {
		if (__get_user(val, p))
			return -EFAULT;
		if (val)
			return 0;
		p++;
	}

	return 1;
}
EXPORT_SYMBOL(check_zeroed_user);
