// SPDX-License-Identifier: GPL-2.0
/*
 * In-tree replacement for the proprietary fpsgo_cus XGF ko.
 *
 * It provides:
 *  - per-frame runtime estimation from XGF render-sector, queue and
 *    dequeue timestamps, with the existing EMA kept up to date;
 *  - per-CPU frame/runtime statistics, enabled by the XGF stat-exchange
 *    callback and exported through /proc/fpsgo_cus.
 */

#include <linux/atomic.h>
#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/types.h>

#include "xgf.h"

/* Ignore queue/dequeue spans that are clearly not a single frame. */
#define FPSGO_CUS_MAX_SAMPLE_NS 100000000ULL

struct fpsgo_cus_cpu_stat {
	atomic64_t frames;
	atomic64_t runtime_ns;
	atomic64_t ema_ns;
};

static struct fpsgo_cus_cpu_stat *fpsgo_cus_stats;
static int fpsgo_cus_cpu_nr;
static atomic_t fpsgo_cus_stat_enabled = ATOMIC_INIT(0);

static void fpsgo_cus_stats_reset(void)
{
	int cpu;

	if (!fpsgo_cus_stats)
		return;

	for (cpu = 0; cpu < fpsgo_cus_cpu_nr; cpu++) {
		atomic64_set(&fpsgo_cus_stats[cpu].frames, 0);
		atomic64_set(&fpsgo_cus_stats[cpu].runtime_ns, 0);
		atomic64_set(&fpsgo_cus_stats[cpu].ema_ns, 0);
	}
}

/*
 * Use the longest visible render path as the runtime sample.  This is the
 * same signal XGF exposes to its callbacks: a frame is bounded by the
 * longest sector, queue or dequeue span, and the frame interval is used as
 * a fallback when no sector has completed yet.
 */
static unsigned long long fpsgo_cus_sample(struct xgf_render *render,
					   unsigned long long ts)
{
	struct xgf_render_sector *sector;
	unsigned long long sample = 0;
	int scanned = 0;

	if (render->curr_ts > render->prev_ts)
		sample = render->curr_ts - render->prev_ts;

	hlist_for_each_entry(sector, &render->sector_head, hlist) {
		unsigned long long span;

		if (sector->tail_ts > sector->head_ts) {
			span = sector->tail_ts - sector->head_ts;
			if (span > sample)
				sample = span;
		}
		if (++scanned >= 8)
			break;
	}

	if (render->queue.end_ts > render->queue.start_ts) {
		unsigned long long span =
			render->queue.end_ts - render->queue.start_ts;

		if (span > sample)
			sample = span;
	}
	if (render->deque.end_ts > render->deque.start_ts) {
		unsigned long long span =
			render->deque.end_ts - render->deque.start_ts;

		if (span > sample)
			sample = span;
	}
	if (!sample && ts > render->curr_ts)
		sample = ts - render->curr_ts;

	if (sample > FPSGO_CUS_MAX_SAMPLE_NS)
		sample = FPSGO_CUS_MAX_SAMPLE_NS;

	return sample;
}

static int fpsgo_cus_est_runtime(pid_t r_pid, struct xgf_render *render,
				 unsigned long long *runtime,
				 unsigned long long ts)
{
	unsigned long long sample, est;
	int cpu;

	(void)r_pid;

	xgf_lockprove(__func__);

	if (!runtime)
		return -EINVAL;
	if (!render) {
		*runtime = 0;
		return -EINVAL;
	}

	sample = fpsgo_cus_sample(render, ts);

	if (render->ema_runtime)
		est = (render->ema_runtime * EMA_DIVIDEND +
		       sample * EMA_REST_DIVIDEND) / EMA_DIVISOR;
	else
		est = sample;

	render->ema_runtime = est;
	*runtime = est;

	if (atomic_read(&fpsgo_cus_stat_enabled) && fpsgo_cus_stats) {
		cpu = get_cpu();
		if (cpu >= 0 && cpu < fpsgo_cus_cpu_nr) {
			atomic64_inc(&fpsgo_cus_stats[cpu].frames);
			atomic64_add(sample, &fpsgo_cus_stats[cpu].runtime_ns);
			atomic64_set(&fpsgo_cus_stats[cpu].ema_ns, est);
		}
		put_cpu();
	}

	return 0;
}

static int fpsgo_cus_stat_xchg(int enable)
{
	if (enable != 0 && enable != 1)
		return -EINVAL;

	if (enable)
		fpsgo_cus_stats_reset();

	atomic_set(&fpsgo_cus_stat_enabled, enable);

	return 0;
}

static int fpsgo_cus_stats_show(struct seq_file *m, void *v)
{
	int cpu;

	seq_printf(m, "enabled %d\n", atomic_read(&fpsgo_cus_stat_enabled));
	seq_printf(m, "cpu frames runtime_ns ema_ns\n");

	for_each_possible_cpu(cpu) {
		if (!fpsgo_cus_stats || cpu >= fpsgo_cus_cpu_nr)
			break;
		seq_printf(m, "%d %lld %lld %lld\n", cpu,
			   (long long)atomic64_read(&fpsgo_cus_stats[cpu].frames),
			   (long long)atomic64_read(&fpsgo_cus_stats[cpu].runtime_ns),
			   (long long)atomic64_read(&fpsgo_cus_stats[cpu].ema_ns));
	}

	return 0;
}

static int fpsgo_cus_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, fpsgo_cus_stats_show, NULL);
}

static const struct file_operations fpsgo_cus_stats_fops = {
	.open = fpsgo_cus_stats_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init fpsgo_cus_init(void)
{
	fpsgo_cus_cpu_nr = num_possible_cpus();
	fpsgo_cus_stats = kcalloc(fpsgo_cus_cpu_nr, sizeof(*fpsgo_cus_stats),
				  GFP_KERNEL);
	if (!fpsgo_cus_stats)
		pr_warn("fpsgo_cus: no memory for stats\n");
	else if (!proc_create("fpsgo_cus", 0444, NULL,
			      &fpsgo_cus_stats_fops))
		pr_warn("fpsgo_cus: failed to create /proc/fpsgo_cus\n");

	xgf_est_runtime_fp = fpsgo_cus_est_runtime;
	xgf_stat_xchg_fp = fpsgo_cus_stat_xchg;
	notify_xgf_ko_ready();

	pr_info("fpsgo_cus: built-in XGF ko registered, cpus=%d\n",
		fpsgo_cus_cpu_nr);

	return 0;
}
late_initcall(fpsgo_cus_init);
